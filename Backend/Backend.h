//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_BACKEND_H
#define GS_BACKEND_2024_2025_BACKEND_H

#include <QTimer>
#include <QDateTime>
#include <QMutex>
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

    struct ThroughputTestParams
    {
        RadioModule *receiveModule;
        uint64_t destinationAddress;
        uint8_t payloadSize;
        uint packetRate;
        uint duration;
        uint8_t transmitOptions;
    };

    struct ThroughputTestResults
    {
        uint8_t payloadSize;
        uint packetRate;
        uint duration;
        uint8_t transmitOptions;
        uint numPacketsReceived;
        float percentReceived;
        float throughput;

        JS_OBJ(
                payloadSize,
                packetRate,
                duration,
                transmitOptions,
                numPacketsReceived,
                percentReceived,
                throughput
                );
    };

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

    void linkTestComplete(LinkTestResults results, int iterationsLeft);
    void runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations, uint8_t repeat, bool loop=false);
    void cancelLinkTest();
    void linkTestFailed();

    void cancelThroughputTest();
    void _runThroughputTest(ThroughputTestParams params);
    void throughputTestComplete();
    void runThroughputTest(const QString& originatingPort, uint64_t destinationAddress, uint8_t payloadSize, uint packetRate, uint duration, uint8_t transmitOptions);
    void sendEnergyDetectCommand(uint16_t msPerChannel);

    void runThroughputTestsWithRange(const QString& originatingPort, uint64_t destinationAddress, QList<QList<int>> params, uint duration, uint8_t transmitOptions);

    void start();
    void flushFiles();
    void getPorts();

    QList<RadioModule *> radioModules;
    int loopCount;

    qint64 throughputTestStartTime{};
    QTimer *throughputTestTimer;
    ThroughputTestParams throughputTestParams{};
    uint8_t throughputTestDummyLoad[255]{};

    QList<QList<int>> throughputTests;
    int throughputTestIndex;


public slots:
    void portOpened(const QSerialPortInfo&, bool);
    void portClosed(const QSerialPortInfo&);
    void throughputTestTimerTicked();
    void runRadioModuleCycles();

signals:
    void foundSerialPorts(QList<QSerialPortInfo>);
    void serialPortOpened(QSerialPortInfo, bool);
    void serialPortClosed(QSerialPortInfo);
    void linkTestDataAvailable(LinkTestResults, int);
    void linkTestFailedSignal();

    void throughputTestDataAvailable(float, uint, uint);

private:
    explicit Backend(QObject *parent = nullptr);

    RadioModule *getModuleWithName(const QString& name);

    WebServer *webServer{};
//    DataSimulator *dataSimulator;
    DataLogger *dataLogger{};

    QTimer *timer{};

    QMutex mutex;

    bool throughputTestShouldStop = false;
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
