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
#include "Utility/DataSimulator.h"

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
    }
    struct Telmetry
    {
        GroundStation::PacketType packetType;
        union data
        {
            GroundStation::RocketTelemPacket *rocketData;
            GroundStation::PayloadTelemPacket *payloadData;
        } data;
    };

    enum RadioModuleType
    {
        Default,
        Rocket,
        Payload
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
    void receiveTelemetry(Backend::Telemetry telemetry);

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
    void newGroundDateTime(std::tm* currentDate);
    void newRocketUptime(uint_fast64_t currentRocketTime);
    void newPayloadUptime(uint_fast64_t currentPayloadTime);

    void throughputTestDataAvailable(float, uint, uint);
    void telemetryAvailable(Backend::Telemetry);

private:
    explicit Backend(QObject *parent = nullptr);

    RadioModule *getModuleWithName(const QString& name);

    WebServer *webServer{};
    DataSimulator *dataSimulator;
    DataLogger *dataLogger{};

    QTimer *timer{};

    QMutex mutex;

    bool throughputTestShouldStop = false;

    // ground date/time
    QTimer *rtcTimer{};
    std::time_t currentGroundEpoch;

    // ground date/time
    QTimer *rtcTimer{};
    std::time_t currentGroundEpoch;
};


#endif //GS_BACKEND_2024_2025_BACKEND_H
