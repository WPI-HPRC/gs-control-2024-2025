//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QTimer>
#include "RadioModule.h"
#include "../Utility/WebServer.h"
#include "../Utility/DataLogger.h"

enum RadioModuleType
{
    Serving,
    Rocket,
    Payload
};

class Backend : public QObject
{
    Q_OBJECT
public:
    static Backend &getInstance()
    {
        static Backend instance;
        return instance;
    }
    Backend(const Backend&) = delete;
    Backend &operator=(const Backend&) = delete;
    void connectToModule(const QString& name, RadioModuleType moduleType);

    void start();
    void flushFiles();

    QList<RadioModule *> radioModules;
    int loopCount;

signals:
    void foundSerialPorts(QList<QSerialPortInfo>);

private:
    explicit Backend(QObject *parent = nullptr);

    void getPorts();

    WebServer *webServer{};
//    DataSimulator *dataSimulator;
    DataLogger *dataLogger{};

    QTimer *timer{};
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
