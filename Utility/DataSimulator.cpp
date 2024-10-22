#include "DataSimulator.h"
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <string>
#include "Utility/json_struct.h"
#include "Utility.h"
#include "Backend/Backend.h"

DataSimulator::DataSimulator(const QString &filePath, WebServer *webServer, QObject *parent)
        : QObject(parent), _webServer(webServer)
{
    file = new QFile();
    file->setFileName(filePath);
}

void DataSimulator::start()
{
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

    nextDocument = parseLine(nextLine());

    sendNextLine();
}

void DataSimulator::stop()
{
    shouldStop = true;
}

QJsonDocument DataSimulator::parseLine(QList<QByteArray> line)
{
    if (line.isEmpty())
    {
        std::cout << "Empty line or failed to read line." << std::endl;
        return {};
    }

    QString jsonString = "{";
    for (int i = 0; i < headers.length(); i++)
    {
        jsonString.append(
                QString::asprintf(R"("%s":%s,)", headers[i].toStdString().c_str(), line[i].toStdString().c_str()));
    }
    jsonString.chop(1);  // Remove the last comma
    jsonString.append("}");

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());

    return jsonDocument;
}

QList<QByteArray> DataSimulator::nextLine()
{
    QByteArray line = file->readLine().trimmed();
    if (line.isEmpty())
    {
        std::cout << "Line is empty" << std::endl;
        return {};
    }

    line.replace("\"", "");
    return line.split(',');
}


void DataSimulator::sendNextLine()
{
    if(shouldStop)
        return;

    QJsonDocument currentDocument = nextDocument;
    nextDocument = parseLine(nextLine());

    int currentTimestamp = currentDocument.object().find("timestamp")->toInt();
    int nextTimestamp = nextDocument.object().find("timestamp")->toInt();

    int dt = nextTimestamp - currentTimestamp;

//    std::cout << "Current timestamp: " << currentTimestamp << std::endl;

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
    if (dt > 20000 || dt < 0)
    {
        sendNextLine();
        return;
    }

    std::string str = currentDocument.toJson(QJsonDocument::Compact).toStdString();

    _webServer->broadcast(QString::fromStdString(str));

    JS::ParseContext context(str);
    GroundStation::RocketTelemPacket rocketPacket;
    JS::Error err = context.parseTo(rocketPacket);

    if(err != JS::Error::NoError)
    {
        std::cout << "Error parsing json: " << context.makeErrorString().c_str() << std::endl;
    }
    else
    {
        Backend::Telemetry telemetry{};
        telemetry.packetType = GroundStation::Rocket;
        telemetry.data.rocketData = &rocketPacket;
        Backend::getInstance().receiveTelemetry(telemetry);
    }

    timer->start(dt);
}