#include "DataSimulator.h"
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

DataSimulator::DataSimulator(const QString &filePath, WebServer *webServer, QObject *parent)
        : QObject(parent), _webServer(webServer)
{
    failed = false;
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
