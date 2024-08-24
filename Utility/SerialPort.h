//
// Created by William Scheirey on 8/20/24.
//

#ifndef GS_BACKEND_2024_2025_SERIALPORT_H
#define GS_BACKEND_2024_2025_SERIALPORT_H

#pragma once

#include "../xbee/XBeeUtility.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QDebug>

#include "DataLogger.h"

#include "../xbee/circularQueue.hpp"

//#define DEBUG_SERIAL
#define SERIAL_PORT_READ_BUF_SIZE 65536

class SerialPort : public QObject
{
Q_OBJECT

private:
    QSerialPort serialPort;

    QSerialPortInfo portInfo;

    CircularQueue<uint8_t> *readQueue;

    char readBuffer[SERIAL_PORT_READ_BUF_SIZE]{};

    uint8_t currentFrame[XBee::MaxFrameBytes]{};

    uint8_t currentFrameByteIndex = 0;

    void connectSignals();

    DataLogger *dataLogger;

    bool nextByteIsEscaped = false;

    void openPort();

public:

    SerialPort(const QSerialPortInfo& port, int baudRate, DataLogger *dataLogger,
               XBee::ApiOptions::ApiOptions apiOptions);

    int write(const char *buf, const int &size);

    size_t read(char *buffer, size_t max_bytes);

    bool isOpen();

    int packetsNotYetRead = 0;

    int currentFrameBytesLeftToRead = -1;

    XBee::ApiOptions::ApiOptions apiOptions;

public slots:

    void errorOccurred(QSerialPort::SerialPortError error);
};


#endif //GS_BACKEND_2024_2025_SERIALPORT_H
