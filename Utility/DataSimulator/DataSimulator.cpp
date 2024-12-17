#include "DataSimulator.h"
#include <iostream>
#include <QFile>
#include <string>
#include "Utility/Utility.h"
#include "Backend/Backend.h"
#include "Utility/TelemetryHandler.h"
#include "protobuf/src/google/protobuf/util/json_util.h"


DataSimulator::DataSimulator(const QString &filePath, WebServer *webServer, const google::protobuf::Descriptor *descriptor,
                             GroundStation::PacketType packetType, QObject *parent)
        : QObject(parent), _webServer(webServer), messageDescriptor(descriptor), packetType(packetType)
{
    if (!messageDescriptor)
    {
        std::cerr << "Error: messageDescriptor is null!" << std::endl;
        return;
    }

    file = new QFile();
    file->setFileName(filePath);

    prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(messageDescriptor);
    if (prototype)
    {
        nextPacket.reset(prototype->New());
    }
}

void DataSimulator::start()
{
    if(file->isOpen())
    {
        file->close();
    }
    if (!file->open(QIODeviceBase::ReadOnly))
    {
        qDebug() << "Failed to open file:" << file->fileName() << "Error:" << file->errorString();
        return;
    }

    headers = nextLine();
    if (headers.isEmpty())
    {
        qDebug() << "Failed to read headers from file:" << file->fileName();
        return;
    }

    shouldStop = false;

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &DataSimulator::sendNextLine);

    nextPacket = parseLine(nextLine());

    sendNextLine();
}

void DataSimulator::stop()
{
    shouldStop = true;
}

std::unique_ptr<google::protobuf::Message> DataSimulator::parseLine(QList<QByteArray> line)
{
    if (line.isEmpty())
    {
        std::cout << "Empty line or failed to read line." << std::endl;
        return {};
    }

    std::unique_ptr<google::protobuf::Message> message(prototype->New());
    const google::protobuf::Reflection *reflection = message->GetReflection();
    const google::protobuf::Descriptor *descriptor = message->GetDescriptor();

    for (int i = 0; i < headers.length(); i++)
    {
        TelemetryHandler::setField(message.get(), reflection, descriptor, headers[i].toStdString(), line[i].toStdString());
    }

    return message;
}

QList<QByteArray> DataSimulator::nextLine()
{
    QByteArray line = file->readLine().trimmed();
    if (line.isEmpty())
    {
        std::cout << "Line is empty" << std::endl;
        return {};
    }

    line.replace("\xEF\xBB\xBF", "");  // Removes the zero-width space (U+200B)

    line.replace("\"", "");
    return line.split(',');
}

void DataSimulator::sendNextLine()
{
    if(shouldStop)
        return;

    std::unique_ptr<google::protobuf::Message> currentPacket = std::move(nextPacket);
    nextPacket = parseLine(nextLine());

    if(!currentPacket || !currentPacket->IsInitialized())
    {
        std::cout << "Packet was not initialized; restarting" << std::endl;
        start();
        return;
    }

    const google::protobuf::Reflection *reflection = currentPacket->GetReflection();
    const google::protobuf::FieldDescriptor *timestampField = messageDescriptor->FindFieldByName("timestamp");

    if (!timestampField || timestampField->type() != google::protobuf::FieldDescriptor::TYPE_UINT32)
    {
        std::cerr << "Error: Timestamp field missing or invalid" << std::endl;
        start();
        return;
    }

    uint32_t currentTimestamp = reflection->GetUInt32(*currentPacket, timestampField);
    uint32_t nextTimestamp = reflection->GetUInt32(*nextPacket, timestampField);

    uint32_t dt = nextTimestamp - currentTimestamp;

    // If the timestamps don't make sense, skip this line
    if (dt > 20000)
    {
        sendNextLine();
        return;
    }

    // Convert current packet to JSON
    std::string jsonString;
    absl::Status status = google::protobuf::util::MessageToJsonString(*currentPacket, &jsonString);
    if (status == absl::OkStatus())
    {
        _webServer->broadcast(QString::fromStdString(jsonString));
    }
    else
    {
        std::cerr << "Error converting packet to JSON string: " << status << std::endl;
    }

    Backend::Telemetry telemetry{};
    telemetry.packetType = packetType;  // Assuming a constant packet type

    if (packetType == GroundStation::PacketType::Rocket)
    {
        telemetry.data.rocketData = (HPRC::RocketTelemetryPacket *) currentPacket.get();
    }
    else
    {
        telemetry.data.payloadData = (HPRC::PayloadTelemetryPacket *) currentPacket.get();
    }

    Backend::getInstance().receiveTelemetry(telemetry);

    if (file->atEnd())
    {
        std::cout << "Reached end of file. Restarting" << std::endl;
        start();
        return;
    }

    timer->start(static_cast<int>(dt));
}
