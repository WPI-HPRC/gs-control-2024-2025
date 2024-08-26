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

public:
    SerialPort(const QSerialPortInfo& port, int baudRate, DataLogger *dataLogger);

    int write(const char *buf, const int &size);
    size_t read(char *buffer, size_t max_bytes);

    bool isOpen();

public slots:
    void errorOccurred(QSerialPort::SerialPortError error);

private:
    void openPort();
    void connectSignals();

    QSerialPort serialPort;
    QSerialPortInfo portInfo;
    DataLogger *dataLogger;
};


#endif //GS_BACKEND_2024_2025_SERIALPORT_H