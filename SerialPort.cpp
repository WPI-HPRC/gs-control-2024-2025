//
// Created by William Scheirey on 8/20/24.
//

#include "SerialPort.h"


#define connectHelper(method_signature) connect(m_serialPort, SIGNAL(method_signature), this, SLOT(method_signature))

#define DEBUG false

SerialPort::SerialPort(QSerialPortInfo port, int baudRate, DataLogger *dataLogger,
                       XBee::ApiOptions::ApiOptions apiOptions) : dataLogger(
        dataLogger), apiOptions(apiOptions)
{
    readQueue = circularQueueCreate<uint8_t>(65536);

    m_serialPort = new QSerialPort();

    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setPort(port);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort->setReadBufferSize(SERIAL_PORT_READ_BUF_SIZE);

#ifndef REQUIRE_XBEE_MODULE
    if (port.isNull())
    {
        std::cout << "No port to open." << std::endl;
        return;
    }
#endif

    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        std::cout << "Opened serial port " << port.manufacturer().toStdString() << " - "
                  << port.portName().toStdString() << " at baud rate " << baudRate << "\n";
        connectSignals();
    }
    else
    {
        qWarning() << "Couldn't open serial port " << port.portName();
        return;
    }

    std::cout.flush();
}

bool SerialPort::isOpen()
{
    return m_serialPort->isOpen();
}

void SerialPort::connectSignals()
{
#if DEBUG
    connectHelper(errorOccurred(QSerialPort::SerialPortError));
#endif
    connectHelper(aboutToClose());
    connectHelper(bytesWritten(qint64));
    connectHelper(channelBytesWritten(int, qint64));
    connectHelper(channelReadyRead(int));
    connectHelper(readChannelFinished());
    connectHelper(readyRead());
}

void SerialPort::errorOccurred(QSerialPort::SerialPortError error)
{
    qDebug() << "Serial error occurred: " << error;
}

int SerialPort::write(const char *buf, const int &size)
{
    return (int) m_serialPort->write(buf, size);
}

size_t SerialPort::read(char *buffer, size_t max_bytes)
{
    size_t bytes_read = m_serialPort->read(buffer, (long long)max_bytes);

    for(int i = 0; i < bytes_read; i++)
    {
        std::string str = QString::asprintf("%02x\n", buffer[i] & 0xFF).toStdString();
        dataLogger->writeToByteFile(str.c_str(), (qint64) str.length());
    }
    dataLogger->flushByteFile();
    return bytes_read;
}