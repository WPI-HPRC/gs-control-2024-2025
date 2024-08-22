//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_RADIOMODULE_H
#define GS_BACKEND_2024_2025_RADIOMODULE_H

#include "xbee/XBeeDevice.h"
#include "DataLogger.h"
#include "SerialPort.h"
#include "Utility.h"
#include "WebServer.h"

class RadioModule : public XBeeDevice
{
public:
    RadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo);

    RadioModule(int baudRate, DataLogger *logger);

    DataLogger *dataLogger{};

    SerialPort *serialPort{};

    QString name;

    DataLogger::Packet lastPacket;

    unsigned int cycleCountsFromFrameID[255]{};

    void configureRadio();

    void readBytes(uint8_t *buffer, size_t length_bytes) override;

    size_t readBytes_uart(char *buffer, size_t max_bytes) override;

    void writeBytes(const char *data, size_t length_bytes) override;

    void packetRead() override;

    void handleReceivePacket(XBee::ReceivePacket::Struct *frame) override;

    void handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame) override;

    void start() override;

    void incorrectChecksum(uint8_t calculated, uint8_t received) override;

    void
    _handleRemoteAtCommandResponse(const uint8_t *frame, uint8_t length_bytes) override;

    void _handleExtendedTransmitStatus(const uint8_t *frame, uint8_t length_bytes) override;

    bool areBytesAvailable() override;

    void sentFrame(uint8_t frameID) override;

    void log(const char *format, ...) override;

    unsigned int cycleCount = 0;

    RocketTelemPacket dummyPacket;
};

class ServingRadioModule
        : public RadioModule // (slaying because this class sends data through the web server so it is serving content so it is serving and if it's serving then it's also slaying)
{
public:
    WebServer *webServer;

    ServingRadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo, WebServer *server);

    ServingRadioModule(int baudRate, DataLogger *logger, WebServer *server);

    void handleReceivePacket(XBee::ReceivePacket::Struct *frame) override;

    void handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame) override;
};


#endif //GS_BACKEND_2024_2025_RADIOMODULE_H
