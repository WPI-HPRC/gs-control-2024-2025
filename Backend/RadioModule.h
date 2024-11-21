//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_RADIOMODULE_H
#define GS_BACKEND_2024_2025_RADIOMODULE_H

#include "../xbee/XBeeDevice.h"
#include "../Utility/DataLogger.h"
#include "../Utility/SerialPort.h"
#include "../Utility/Utility.h"
#include "../Utility/WebServer.h"

struct LinkTestResults
{
    uint64_t destinationAddress;
    uint16_t payloadSize;
    uint16_t iterations;
    uint16_t success;
    uint16_t retries;
    uint8_t result;
    uint8_t RR;
    uint8_t maxRssi;
    uint8_t minRssi;
    uint8_t avgRssi;
    uint8_t noiseFloor;
    JS_OBJ(
            destinationAddress,
            payloadSize,
            iterations,
            success,
            retries,
            result,
            RR,
            maxRssi,
            minRssi,
            avgRssi,
            noiseFloor
    );
};

class RadioModule : public XBeeDevice
{
public:
    RadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo);
    RadioModule(int baudRate, DataLogger *logger);
    void setBaudRate(int baudRate);

    void configureRadio();

    size_t readBytes_uart(char *buffer, size_t max_bytes) override;
    void writeBytes(const char *data, size_t length_bytes) override;

    void sendLinkTestRequest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations) override;
    void handleLinkTest(XBee::ExplicitRxIndicator::LinkTest data) override;
    void handleEnergyDetectResponse(uint8_t *energyValues, uint8_t numChannels) override;

    void handleReceivePacket(XBee::ReceivePacket::Struct *frame) override;
    void handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame) override;
    void
    _handleRemoteAtCommandResponse(const uint8_t *frame, uint8_t length_bytes) override;
    void _handleExtendedTransmitStatus(const uint8_t *frame, uint8_t length_bytes) override;

    void _handleTransmitStatus(uint8_t frameID, uint8_t statusCode) override;

    void _handleAtCommandResponse(const uint8_t *frame, uint8_t length_bytes) override;

    void incorrectChecksum(uint8_t calculated, uint8_t received) override;

    void sentFrame(uint8_t frameID) override;

    void log(const char *format, ...) override;

    void disconnectPort();

    void connectPort();

    uint32_t packetsReceivedCount = 0;
    uint64_t bytesReceivedCount = 0;
    uint32_t droppedPacketsCount = 0;

    DataLogger *dataLogger{};
    SerialPort *serialPort{};

    QString name;

    DataLogger::Packet lastPacket;
    unsigned int cycleCountsFromFrameID[255]{};
    unsigned int cycleCount = 0;

    uint8_t lastNoiseFloor{};

    int linkTestsLeft{};

    bool receivingThroughputTest = false;
    uint throughputTestPacketsReceived = 0;

private:
    void handlingFrame(const uint8_t *frame) override;
};

class ServingRadioModule
        : public RadioModule // (slaying because this class sends data through the web server so it is serving content so it is serving and if it's serving then it's also slaying)
{
public:
    ServingRadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo, WebServer *server);
    ServingRadioModule(int baudRate, DataLogger *logger, WebServer *server);

    void handleReceivePacket(XBee::ReceivePacket::Struct *frame) override;
    void handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame) override;

    WebServer *webServer;
};


class RocketTestModule : public RadioModule
{
public:
    RocketTestModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo) : RadioModule(baudRate, logger,
                                                                                                      portInfo)
    {}

    RocketTestModule(int baudRate, DataLogger *logger) : RadioModule(baudRate, logger)
    {}

    void didCycle() override;

    GroundStation::RocketTxPacket packet;
};

class PayloadTestModule : public RadioModule
{
public:
    PayloadTestModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo) : RadioModule(baudRate, logger,
                                                                                                       portInfo)
    {}

    PayloadTestModule(int baudRate, DataLogger *logger) : RadioModule(baudRate, logger)
    {}

    void didCycle() override;

    GroundStation::PayloadTxPacket packet;
};


#endif //GS_BACKEND_2024_2025_RADIOMODULE_H
