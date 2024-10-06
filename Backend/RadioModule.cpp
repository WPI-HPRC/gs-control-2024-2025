//
// Created by William Scheirey on 8/20/24.
//

#include "RadioModule.h"
#include "Backend.h"

#include <QSerialPortInfo>
#include <regex>
#include <QJsonDocument>

QSerialPortInfo getTargetPort()
{
    QList serialPorts = QSerialPortInfo::availablePorts();

    QSerialPortInfo targetPort;

#ifdef DEBUG_SERIAL
    std::cout << "Available baud rates: \n";
    for (auto &baudRate: QSerialPortInfo::standardBaudRates())
    {
        std::cout << "\t" << baudRate << "\n";
    }

    std::cout << "\nFound serial ports: \n";
#endif

    for (auto &port: serialPorts)
    {
#ifdef DEBUG_SERIAL
        std::cout << "\n" << port.portName().toStdString() << "\n";
        std::cout << "\tManufacturer: " << port.manufacturer().toStdString() << "\n";
        std::cout << "\tSystem location: " << port.systemLocation().toStdString() << "\n";
        std::cout << "\tSerial number: " << port.serialNumber().toStdString() << "\n";
        std::cout.flush();
#endif

        if (!port.portName().contains("cu.") && port.manufacturer().contains("Digi") ||
            port.manufacturer().contains("FTDI"))
        {
            targetPort = port;
        }
    }

    return targetPort;
}

DataLogger::Packet parsePacket(const uint8_t *frame)
{
    std::string str;

    // This way of assigning the packet type seems redundant, but the packetType byte can take on any value from 0-255; we want to set it to an enum value that we understand
    GroundStation::PacketType packetType;

    switch (frame[0])
    {
        case GroundStation::Rocket:
            str = JS::serializeStruct(*(GroundStation::RocketTelemPacket *) (&frame[1]));
            packetType = GroundStation::Rocket;
            break;
        case GroundStation::Payload:
            str = JS::serializeStruct(*(GroundStation::PayloadTelemPacket *) (&frame[1]));
            packetType = GroundStation::Payload;
            break;
        default:
            str = "";
            packetType = GroundStation::Unknown;
            break;
    }

    str = std::regex_replace(str, std::regex("nan"), "0");
    str = std::regex_replace(str, std::regex("inf"), "0");

    return {str, packetType};
}

void RadioModule::disconnectPort()
{
    serialPort->close();
}

void RadioModule::connectPort()
{
    serialPort->open();
}

void RadioModule::configureRadio()
{
//    std::cout << "Configuring radio" << std::endl; // Does nothing right now
//    setParameter(XBee::AtCommand::ApiOptions, 0x02);
//    setParameterRemote(, XBee::AtCommand::PowerLevel, 0x02);
}

RadioModule::RadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo) : XBeeDevice(SerialInterface::UART)
{
    dataLogger = logger;

    serialPort = new SerialPort(portInfo, baudRate, dataLogger);

    sendTransmitRequestsImmediately = false;

    sendFramesImmediately = false;

    logWrongChecksums = true;

    linkTestsLeft = 0;

    configureRadio();
}

RadioModule::RadioModule(int baudRate, DataLogger *logger) : XBeeDevice(SerialInterface::UART)
{
    QSerialPortInfo targetPort = getTargetPort();

    if (targetPort.isNull())
    {
        qDebug() << "Couldn't find radio module";
        exit(1);
    }

    *this = RadioModule(baudRate, logger, targetPort);
}

void RadioModule::writeBytes(const char *data, size_t length_bytes)
{
    if(!serialPort->isOpen())
        return;
#ifndef REQUIRE_XBEE_MODULE
    if(!serialPort->isOpen())
    {
        return;
    }
#endif
    int bytes_written = serialPort->write(data, (int) length_bytes);

    dataLogger->writeToTextFile("Writing: ");
    for (int i = 0; i < length_bytes; i++)
    {
        dataLogger->writeToTextFile(QString::asprintf("%02x ", data[i] & 0xFF));
    }
    dataLogger->writeToTextFile("\n");
    dataLogger->flushTextFile();


    if (bytes_written != length_bytes)
    {
        log("FAILED TO WRITE ALL BYTES. EXPECTED %d, RECEIVED %d\n", length_bytes, bytes_written);
    }

}

size_t RadioModule::readBytes_uart(char *buffer, size_t max_bytes)
{
    if(!serialPort->isOpen())
        return 0;

    return serialPort->read(buffer, max_bytes);
}

void RadioModule::handleReceivePacket(XBee::ReceivePacket::Struct *frame)
{
    lastPacket = parsePacket(frame->data);
    dataLogger->dataReady(lastPacket.data.c_str(), lastPacket.packetType);
}

void RadioModule::handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame)
{
    lastPacket = parsePacket(frame->data);
    dataLogger->dataReady(lastPacket.data.c_str(), lastPacket.packetType, frame->negativeRssi);
}

void RadioModule::incorrectChecksum(uint8_t calculated, uint8_t received)
{
    std::string str = QString::asprintf("\nWRONG CHECKSUM. calculated: %02x, received: %02x\n\n", calculated & 0xFF,
                                        received & 0xFF).toStdString();

//    log(str.c_str());

//    dataLogger->writeToByteFile(str.c_str(), str.length());
    dataLogger->writeToTextFile(str.c_str(), str.length());

//    dataLogger->flushByteFile();
    dataLogger->flushTextFile();
}

void RadioModule::log(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vprintf(format, args);
    fflush(stdout);

    char buff[256];
    vsnprintf(buff, sizeof(buff), format, args);
    std::string str = buff;

    dataLogger->writeToTextFile(str.c_str(), str.length());

    dataLogger->flushTextFile();

    va_end(args);
}

void RadioModule::sendLinkTestRequest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations)
{
    std::cout << "Sending link test request" << std::endl;
    sendEnergyDetectCommand(150);
    XBeeDevice::sendLinkTestRequest(destinationAddress, payloadSize, iterations);
}

void RadioModule::_handleExtendedTransmitStatus(const uint8_t *frame, uint8_t length_bytes)
{
    using namespace XBee::ExtendedTransmitStatus;

    auto *data = (Struct *)(&frame[4]);

    auto *status = (Struct *)(&frame[BytesBeforeFrameID]);

    QJsonObject json;
    json.insert("FrameID", status->frameID);
    json.insert("RetryCount", status->retryCount);
    json.insert("DeliveryStatus", status->deliveryStatus);
    json.insert("Discovery", status->discovery);
    json.insert("CycleCount", (int)cycleCountsFromFrameID[status->frameID]);

//    log("Transmit status for frame ID %03x: %02x. RetryCount: %03x, Discovery: %02x\n", status->frameID, status->deliveryStatus, status->retryCount, status->discovery);

    dataLogger->logTransmitStatus(json);

    std::cout << "Got transmit status: " << std::hex << (int)status->deliveryStatus << std::endl;

    if(status->deliveryStatus != 0x00)
    {
        if(linkTestsLeft != 0)
        {
            std::cout << "Link test failed!" << std::endl;
            Backend::getInstance().linkTestFailed();
        }
    }
}

void RadioModule::handleLinkTest(XBee::ExplicitRxIndicator::LinkTest data)
{
    LinkTestResults results{};
    memcpy(&results, &data, sizeof(data));
    results.noiseFloor = lastNoiseFloor;

    std::string str = JS::serializeStruct(results);

    QJsonObject obj = QJsonDocument::fromJson(str.c_str()).object();

    dataLogger->logLinkTest(obj);

    Backend::getInstance().linkTestComplete(results, linkTestsLeft);

    /*
    log("Finished link test.\n\tPayload Size: %d\n\tIterations: %d\n\tSuccess: %d\n\tRetries: %d\n\tResult: %02x\n\tRR: %d\n\tMax RSSI: %d\n\tMin RSSI: %d\n\tAvg RSSI: %d\n",
        data.payloadSize, data.iterations, data.success, data.retries, data.result, data.RR, data.maxRssi, data.minRssi, data.avgRssi
        );

     */
    std::cout << "Got link test result" << std::endl;

    if(linkTestsLeft != 0)
    {
        std::cout << "Sending new link test request. " << std::dec << ((int)linkTestsLeft) << " left" << std::endl;
        sendLinkTestRequest(data.destinationAddress, data.payloadSize, data.iterations);
        linkTestsLeft--;
    }
}

void RadioModule::handleEnergyDetectResponse(uint8_t *energyValues, uint8_t numChannels)
{
    float average = 0;

    for(int i = 0; i < numChannels; i++)
    {
        average += (float)energyValues[i]/numChannels;
    }
    lastNoiseFloor = (uint8_t)average;
}

void RadioModule::sentFrame(uint8_t frameID)
{
    cycleCountsFromFrameID[frameID] = cycleCount;
}

void RadioModule::_handleRemoteAtCommandResponse(const uint8_t *frame, uint8_t length_bytes)
{
    uint16_t command = getRemoteAtCommand(frame);

    uint64_t address = getAddressBigEndian(&frame[XBee::RemoteAtCommandResponse::BytesBeforeAddress]);

    log("Remote AT response from %016llx: ", (unsigned long long) address);
    if (command == XBee::AtCommand::SupplyVoltage)
    {
        uint16_t voltage = frame[XBee::RemoteAtCommandResponse::BytesBeforeCommandData] << 8 |
                           frame[XBee::RemoteAtCommandResponse::BytesBeforeCommandData + 1];

        log("voltage = %f mV\n", (float) voltage / 1000);
        return;
    }

    log("%c%c: ", (command & 0xFF00) >> 8, command & 0x00FF);
    for (uint8_t i = 0; i < length_bytes - XBee::RemoteAtCommandResponse::PacketBytes; i++)
    {
        log("%02x ", (int) (frame[XBee::RemoteAtCommandResponse::BytesBeforeCommandData + i] & 0xFF));
    }

    log("\n");
}

ServingRadioModule::ServingRadioModule(int baudRate, DataLogger *logger, const QSerialPortInfo &portInfo, WebServer *server): RadioModule(baudRate, logger, portInfo)
{
    webServer = server;
}

ServingRadioModule::ServingRadioModule(int baudRate, DataLogger *logger, WebServer *server): RadioModule(baudRate, logger)
{
    webServer = server;
}

void ServingRadioModule::handleReceivePacket64Bit(XBee::ReceivePacket64Bit::Struct *frame)
{
    log("RSSI: -%dbm\n", frame->negativeRssi);
    RadioModule::handleReceivePacket64Bit(frame);

    webServer->broadcast(QString::fromStdString(lastPacket.data));
}

void ServingRadioModule::handleReceivePacket(XBee::ReceivePacket::Struct *frame)
{
    RadioModule::handleReceivePacket(frame);

    webServer->broadcast(QString::fromStdString(lastPacket.data));
}

void RocketTestModule::didCycle()
{
    constexpr int interval = 4;
    if (cycleCount % interval == 0)
    {
        packet.timestamp = cycleCount / interval;
        packet.epochTime = QDateTime::currentMSecsSinceEpoch();

        sendTransmitRequestCommand(GROUND_STATION_ADDR, (uint8_t *)&packet, sizeof(packet));
    }
    cycleCount++;
}

void PayloadTestModule::didCycle()
{
    constexpr int interval = 4;
    if (cycleCount % interval == 0)
    {
        packet.timestamp = cycleCount / interval;
        packet.epochTime = QDateTime::currentMSecsSinceEpoch();

        sendTransmitRequestCommand(GROUND_STATION_ADDR, (uint8_t *)&packet, sizeof(packet));
    }
    cycleCount++;
}