//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QTimer>

#include "RadioModule.h"
#include "WebServer.h"
//#include "DataSimulator.h"
#include "DataLogger.h"
//#include "ByteParser.h"

class Backend : public QObject
{
public:
    explicit Backend(QObject *parent = nullptr);

    QList<RadioModule *> radioModules;
    int loopCount;

    void flushFiles();

private:
    WebServer *webServer;
//    DataSimulator *dataSimulator;
    DataLogger *dataLogger;

    QTimer *timer;
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
