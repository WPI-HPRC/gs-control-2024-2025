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
    connectHelper(baudRateChanged(qint32, QSerialPort::Directions));
    connectHelper(breakEnabledChanged(bool));
    connectHelper(dataBitsChanged(QSerialPort::DataBits));
    connectHelper(dataTerminalReadyChanged(bool));
    connectHelper(errorOccurred(QSerialPort::SerialPortError));
    connectHelper(flowControlChanged(QSerialPort::FlowControl));
    connectHelper(parityChanged(QSerialPort::Parity));
    connectHelper(requestToSendChanged(bool));
    connectHelper(stopBitsChanged(QSerialPort::StopBits));
#endif
    connectHelper(aboutToClose());
    connectHelper(bytesWritten(qint64));
    connectHelper(channelBytesWritten(int, qint64));
    connectHelper(channelReadyRead(int));
    connectHelper(readChannelFinished());
    connectHelper(readyRead());
}

void SerialPort::baudRateChanged(qint32 baudRate, QSerialPort::Directions directions)
{
    qDebug() << "Baud rate changed to: " << baudRate << " direction: " << directions;
}

void SerialPort::breakEnabledChanged(bool set)
{
    qDebug() << "Break enabled changed to: " << (set ? "TRUE" : "FALSE");
}

void SerialPort::dataBitsChanged(QSerialPort::DataBits dataBits)
{
    qDebug() << "Data bits changed to: " << dataBits;
}

void SerialPort::dataTerminalReadyChanged(bool set)
{
    qDebug() << "Data terminal ready changed to: " << (set ? "TRUE" : "FALSE");

}

void SerialPort::errorOccurred(QSerialPort::SerialPortError error)
{
    qDebug() << "Serial error occurred: " << error;
}

void SerialPort::flowControlChanged(QSerialPort::FlowControl flow)
{
    qDebug() << "Flow control changed to: " << flow;
}

void SerialPort::parityChanged(QSerialPort::Parity parity)
{
    qDebug() << "Parity changed to: " << parity;
}

void SerialPort::requestToSendChanged(bool set)
{
    qDebug() << "Request to sendFrame changed to: " << (set ? "TRUE" : "FALSE");
}

void SerialPort::stopBitsChanged(QSerialPort::StopBits stopBits)
{
    qDebug() << "Stop bits changed to: " << stopBits;
}

void SerialPort::aboutToClose()
{
#if DEBUG
    qDebug() << "About to close";
#endif
}

void SerialPort::bytesWritten(qint64 bytes)
{
#if DEBUG
    //    qDebug() << "Bytes written: " << bytes;
#endif
}

void SerialPort::channelBytesWritten(int channel, qint64 bytes)
{
#if DEBUG
    qDebug() << "Channel bytes written. Channel: " << channel << ", bytes: " << bytes;
#endif
}

void SerialPort::channelReadyRead(int channel)
{
#if DEBUG
    qDebug() << "Channel ready read: " << channel;
#endif
}

void SerialPort::readChannelFinished()
{
#if DEBUG
    qDebug() << "Read channel finished";
#endif
}

void SerialPort::readyRead()
{
    qint64 bytes_transferred = m_serialPort->read(readBuffer, 256);

    // This is pretty much all just for logging purposes
    for (unsigned int i = 0; i < bytes_transferred; ++i)
    {
        char c = readBuffer[i];

        // Just for logging purposes
        if (!(currentFrameBytesLeftToRead > 0 || c == XBee::StartDelimiter))
        {
            std::string str = QString::asprintf("%02x\n", c & 0xFF).toStdString();
            dataLogger->writeToByteFile(str.c_str(), (qint64) str.length());
        }

        if (currentFrameBytesLeftToRead > 0 || c == XBee::StartDelimiter)
        {
            if ((currentFrameBytesLeftToRead <= 0) ||
                (apiOptions == XBee::ApiOptions::ApiWithEscapes && c == XBee::StartDelimiter))
            {
                currentFrameByteIndex = 0;
                currentFrameBytesLeftToRead = 3;
            }

            for (; i < bytes_transferred && currentFrameBytesLeftToRead > 0; i++)
            {
                auto byte = (uint8_t) readBuffer[i];

                circularQueuePush(readQueue, byte);

                if (apiOptions == XBee::ApiOptions::ApiWithEscapes)
                {
                    if (byte == XBee::EscapeCharacter)
                    {
                        nextByteIsEscaped = true;
                        continue;
                    }
                }
                if (nextByteIsEscaped)
                {
                    byte = byte ^ XBee::EscapeXorByte;
                    nextByteIsEscaped = false;
                }

                currentFrame[currentFrameByteIndex++] = byte;

                std::string str = QString::asprintf("%02x ", byte & 0xFF).toStdString();
                dataLogger->writeToByteFile(str.c_str(), (qint64) str.length());

                currentFrameBytesLeftToRead -= 1;

                if (currentFrameByteIndex == 3)
                {
                    currentFrameBytesLeftToRead = currentFrame[2] + 1;
                }
            }
        }

        if (currentFrameBytesLeftToRead == 0)
        {
            /*
            std::cout << "Frame: ";
            for (int j = 0; j < currentFrame[2] + 4; j++)
            {
                std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) (currentFrame[j] & 0xFF) << " ";
            }
            std::cout << std::endl;
             */
            currentFrameBytesLeftToRead = -1;

            dataLogger->writeToByteFile("\n", 1);

            i -= 1; // Subtract 1 because i will be incremented at the end of this iteration of the for loop
        }
    }

    dataLogger->flushByteFile();
}

int SerialPort::write(const char *buf, const int &size)
{
    return (int) m_serialPort->write(buf, size);
}

void SerialPort::read(uint8_t *buffer, size_t length_bytes)
{
    circularQueuePop(readQueue, buffer, length_bytes);
}

bool SerialPort::areBytesAvailable()
{
    return isCircularQueueEmpty(readQueue);
}