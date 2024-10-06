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
#include "WebServer.h"

class DataSimulator : public QObject
{
Q_OBJECT

public:
    DataSimulator(const QString &filePath, WebServer *webServer, QObject *parent = nullptr);
    void start();
    void stop();

private:
    WebServer *_webServer;
    QFile *file;

    QList<QByteArray> nextLine();

    QJsonDocument parseLine(QList<QByteArray> line);

    QList<QByteArray> headers;
    QTimer *timer;
    QJsonDocument nextDocument;

    bool shouldStop = false;

public slots:

    void sendNextLine();
};


#endif //XBEE_BACKEND_CPP_DATASIMULATOR_H
