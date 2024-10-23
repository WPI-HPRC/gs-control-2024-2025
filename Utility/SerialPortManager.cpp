//
// Created by William Scheirey on 8/27/24.
//

#include "SerialPortManager.h"

SerialPortManager::SerialPortManager(QObject *parent): QObject(parent)
{
    thread = new QThread();
}

void SerialPortManager::openPort(const QString &port, Backend::RadioModuleType type, int baudRate)
{
    Backend::getInstance().connectToModule(port, type, baudRate);
}

void SerialPortManager::closePort(const QString &port)
{
    Backend::getInstance().disconnectFromModule(port);
}