#include "DataSimulator.h"
#include <iostream>
#include <QFile>
#include <string>
#include "Utility.h"
#include "Backend/Backend.h"
#include "TelemetryHandler.h"
#include <google/protobuf/util/json_util.h>

DataSimulator::DataSimulator(const QString &filePath, WebServer *webServer, QObject *parent)
        : QObject(parent), _webServer(webServer)
{
    file = new QFile();
    file->setFileName(filePath);
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

HPRC::RocketTelemetryPacket DataSimulator::parseLine(QList<QByteArray> line)
{
    if (line.isEmpty())
    {
        std::cout << "Empty line or failed to read line." << std::endl;
        return {};
    }

    HPRC::RocketTelemetryPacket packet;
    const google::protobuf::Descriptor *descriptor = HPRC::RocketTelemetryPacket::GetDescriptor();
    const google::protobuf::Reflection *reflection = HPRC::RocketTelemetryPacket::GetReflection();

    for (int i = 0; i < headers.length(); i++)
    {
        bool success = TelemetryHandler::setField(&packet, reflection, descriptor, headers[i].toStdString(), line[i].toStdString());
        if(success)
        {
//            std::cout << "Successfully set " << headers[i].toStdString() << " to " << line[i].toStdString() << std::endl;
        }
        else
        {
//            std::cout << "Could not set " << headers[i].toStdString() << " to " << line[i].toStdString() << std::endl;
        }
    }

    return packet;
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

    HPRC::RocketTelemetryPacket currentPacket = nextPacket;
    nextPacket = parseLine(nextLine());

    if(!currentPacket.IsInitialized())
    {
        start();
        return;
    }

    uint32_t currentTimestamp = currentPacket.timestamp();
    uint32_t nextTimestamp = nextPacket.timestamp();

    uint32_t dt = nextTimestamp - currentTimestamp;

    /*
    if (!file->canReadLine())
    {
        if (file->atEnd())
        {
            std::cout << "Reached end of file." << std::endl;
        }
        else
        {
            std::cout << "Can't read line: " << file->errorString().toStdString() << std::endl;
        }
        timer->stop();
        return;
    }
     */

    // If the timestamps don't make sense, just skip this line
    if (dt > 20000)
    {
        sendNextLine();
        return;
    }

    std::string str{};
    absl::Status status = google::protobuf::util::MessageToJsonString(currentPacket, &str);
    if(status != absl::OkStatus())
    {
        std::cout << "Error converting rocket packet to JSON string: " << status << std::endl;
    }

    _webServer->broadcast(QString::fromStdString(str));

    Backend::Telemetry telemetry{};
    telemetry.packetType = GroundStation::Rocket;
    telemetry.data.rocketData = &currentPacket;
    Backend::getInstance().receiveTelemetry(telemetry);

    timer->start(dt);
}
