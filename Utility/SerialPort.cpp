//
// Created by William Scheirey on 8/20/24.
//

#include "SerialPort.h"
#include <QTimer>


#define connectHelper(method_signature) connect(&serialPort, SIGNAL(method_signature), this, SLOT(method_signature))

#define DEBUG false
//#define WRITE_BYTES_TO_FILE

void SerialPort::openPort()
{
    if (serialPort.open(QIODevice::ReadWrite))
    {
        std::cout << "Opened serial port " << portInfo.portName().toStdString()
        << " at baud rate " << serialPort.baudRate() << "\n";
        connectSignals();
    }
    else
    {
        QTimer::singleShot(500, [this]()
        {
            this->openPort();
        });
    }
}

SerialPort::SerialPort(const QSerialPortInfo& port, int baudRate, DataLogger *dataLogger,
                       XBee::ApiOptions::ApiOptions apiOptions) : dataLogger(
        dataLogger), apiOptions(apiOptions), portInfo(port)
{
    readQueue = circularQueueCreate<uint8_t>(65536);

    serialPort.setBaudRate(baudRate);
    serialPort.setPort(port);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setReadBufferSize(SERIAL_PORT_READ_BUF_SIZE);

    openPort();

    std::cout.flush();
}

bool SerialPort::isOpen()
{
    return serialPort.isOpen();
}

void SerialPort::connectSignals()
{
    connectHelper(errorOccurred(QSerialPort::SerialPortError));
}

void SerialPort::errorOccurred(QSerialPort::SerialPortError error)
{
    if(error == QSerialPort::ResourceError)
    {
        qDebug() << "Serial port " << portInfo.portName() << " closed unexpectedly";
        serialPort.close();
        QTimer::singleShot(1000, [this]()
        {
            this->openPort();
        });
    }
    else if(error == QSerialPort::DeviceNotFoundError)
    {
        qDebug() << "Serial port " << portInfo.portName() << " not found";
    }
    else if (error == QSerialPort::NoError)
    {

    }
    else
    {
        qDebug() << "Serial port error occurred for " << portInfo.portName() << ": " << error;

    }
}

int SerialPort::write(const char *buf, const int &size)
{
    return (int) serialPort.write(buf, size);
}

size_t SerialPort::read(char *buffer, size_t max_bytes)
{
    size_t bytes_read = serialPort.read(buffer, (long long)max_bytes);

#ifdef WRITE_BYTES_TO_FILE
    for(int i = 0; i < bytes_read; i++)
    {
        std::string str = QString::asprintf("%02x\n", buffer[i] & 0xFF).toStdString();
        dataLogger->writeToByteFile(str.c_str(), (qint64) str.length());
    }
    dataLogger->flushByteFile();
#endif
    return bytes_read;
}