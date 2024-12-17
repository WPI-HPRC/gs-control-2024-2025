//
// Created by William Scheirey on 3/13/24.
//

#ifndef XBEE_BACKEND_CPP_DATASIMULATOR_H
#define XBEE_BACKEND_CPP_DATASIMULATOR_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QJsonDocument>
#include <QTimer>
#include "Utility/WebServer.h"

#include "generated/telemetry/RocketTelemetryPacket.pb.h"
#include "Utility/Utility.h"

class DataSimulator : public QObject
{
Q_OBJECT

public:
    DataSimulator(const QString &filePath, WebServer *webServer, const google::protobuf::Descriptor *messageDescriptor, GroundStation::PacketType packetType, QObject *parent = nullptr);
    void start();
    void stop();

private:
    WebServer *_webServer;
    QFile *file;

    QList<QByteArray> nextLine();

    std::unique_ptr<google::protobuf::Message> parseLine(QList<QByteArray> line);

    QList<QByteArray> headers;
    QTimer *timer;

    const google::protobuf::Message *prototype;
    const google::protobuf::Descriptor *messageDescriptor;  // Descriptor of the Protobuf message type
    std::unique_ptr<google::protobuf::Message> nextPacket;  // Dynamically allocated message instance

    bool shouldStop = false;

    GroundStation::PacketType packetType;

public slots:

    void sendNextLine();
};


#endif //XBEE_BACKEND_CPP_DATASIMULATOR_H
