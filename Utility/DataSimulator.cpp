#include "DataSimulator.h"
#include <iostream>
#include <QFile>
#include <QTimer>
#include <QJsonDocument>

DataSimulator::DataSimulator(const QString &filePath, int dataIntervalMs, WebServer *webServer, QObject *parent)
        : QObject(parent), dataIntervalMs(dataIntervalMs), _webServer(webServer)
{
    file = new QFile();
    file->setFileName(filePath);

    if (!file->open(QIODeviceBase::ReadOnly))
    {
        qDebug() << "Failed to open file:" << filePath << "Error:" << file->errorString();
        return;
    }

    _webServer = webServer;

    headers = nextLine();
    if (headers.isEmpty())
    {
        qDebug() << "Failed to read headers from file:" << filePath;
        return;
    }

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &DataSimulator::sendNextLine);

    nextDocument = parseLine(nextLine());

    sendNextLine();
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

    if (dt < 20000 && dt > 0)
    {
        std::cout << "dt: " << dt << std::endl;
    }
    else
    {
        sendNextLine();
        return;
    }


    _webServer->broadcast(QString::fromStdString(currentDocument.toJson(QJsonDocument::Compact).toStdString()));
    timer->start(dt);
}
