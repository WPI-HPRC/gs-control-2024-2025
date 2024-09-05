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
    Default,
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
    bool connectToModule(const QString& name, RadioModuleType moduleType);
    void disconnectFromModule(const QString& name);
    bool moduleExistsWithName(const QString &name);

    void runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations);

    void start();
    void flushFiles();

    QList<RadioModule *> radioModules;
    int loopCount;

public slots:
    void portOpened(const QSerialPortInfo&, bool);
    void portClosed(const QSerialPortInfo&);

signals:
    void foundSerialPorts(QList<QSerialPortInfo>);
    void serialPortOpened(QSerialPortInfo, bool);
    void serialPortClosed(QSerialPortInfo);

private:
    explicit Backend(QObject *parent = nullptr);

    void getPorts();
    RadioModule *getModuleWithName(const QString& name);

    WebServer *webServer{};
//    DataSimulator *dataSimulator;
    DataLogger *dataLogger{};

    QTimer *timer{};
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
