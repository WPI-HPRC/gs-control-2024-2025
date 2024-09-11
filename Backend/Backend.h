//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QTimer>
#include "RadioModule.h"
#include "../Utility/WebServer.h"
#include "../Utility/DataLogger.h"
#include "Utility/json_struct.h"

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

    void runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations, uint8_t repeat, bool loop=false);
    void cancelLinkTest();
    void linkTestFailed();
    void sendEnergyDetectCommand(uint16_t msPerChannel);

    void start();
    void flushFiles();
    void getPorts();

    QList<RadioModule *> radioModules;
    int loopCount;

    void linkTestComplete(LinkTestResults results, int iterationsLeft);

public slots:
    void portOpened(const QSerialPortInfo&, bool);
    void portClosed(const QSerialPortInfo&);

signals:
    void foundSerialPorts(QList<QSerialPortInfo>);
    void serialPortOpened(QSerialPortInfo, bool);
    void serialPortClosed(QSerialPortInfo);
    void linkTestDataAvailable(LinkTestResults, int);
    void linkTestFailedSignal();

private:
    explicit Backend(QObject *parent = nullptr);

    RadioModule *getModuleWithName(const QString& name);

    WebServer *webServer{};
//    DataSimulator *dataSimulator;
    DataLogger *dataLogger{};

    QTimer *timer{};
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
