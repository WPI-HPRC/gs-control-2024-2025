//
// Created by William Scheirey on 3/13/24.
//

#ifndef XBEE_BACKEND_CPP_DATASIMULATOR_H
#define XBEE_BACKEND_CPP_DATASIMULATOR_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include "WebServer.h"

class DataSimulator : public QObject
{
Q_OBJECT

public:
    DataSimulator(const QString &filePath, int dataIntervalMs, WebServer *webServer, QObject *parent = nullptr);

private:
    WebServer *_webServer;
    int dataIntervalMs;
    QFile *file;

    QList<QByteArray> nextLine();

    QJsonDocument parseLine(QList<QByteArray> line);

    QList<QByteArray> headers;
    QTimer *timer;
    QJsonDocument nextDocument;


public slots:

    void sendNextLine();
};


#endif //XBEE_BACKEND_CPP_DATASIMULATOR_H
