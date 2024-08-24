//
// Created by William Scheirey on 8/20/24.
//

#include "Backend.h"
#include <QSerialPort>
//#define SIMULATE_DATA

QSerialPortInfo getTargetPort(const QString& portName)
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

        if (port.portName().contains(portName))
        {
            targetPort = port;
        }
    }

    return targetPort;
}

void Backend::getPorts()
{
    QList serialPorts = QSerialPortInfo::availablePorts();
    emit foundSerialPorts(serialPorts);
}

void Backend::flushFiles()
{
    for (RadioModule *radioModule: this->radioModules)
    {
        radioModule->dataLogger->flushDataFiles();
        radioModule->dataLogger->flushByteFile();
        radioModule->dataLogger->flushTextFile();
    }
}

void Backend::connectToModule(const QString& name, RadioModuleType moduleType)
{
    std::cout << "Trying to connect to module " << name.toStdString() << std::endl;
    QSerialPortInfo targetPort = getTargetPort(name);
    if(targetPort.isNull())
    {
        qDebug() << "Could not find module " << name;
        QTimer::singleShot(1000, [this, name, moduleType]()
        {
           this->connectToModule(name, moduleType);
        });
        return;
    }
    RadioModule *module;
    switch(moduleType)
    {
        case Serving:
            module = new ServingRadioModule(921600, new DataLogger(), targetPort, new WebServer(8001));
            break;
        case Rocket:
            module = new RocketTestModule(921600, new DataLogger(), targetPort);
            break;
        case Payload:
            module = new PayloadTestModule(921600, new DataLogger(), targetPort);
            break;
        default:
            module = new RadioModule(921600, new DataLogger(), targetPort);
    }
    radioModules.append(module);
    std::cout << "Found module " << name.toStdString() << std::endl;
}

void Backend::start()
{
    getPorts();
#ifdef SIMULATE_DATA
    webServer = new WebServer(8001);

    dataSimulator = new DataSimulator(
            "/Users/will/Desktop/irec_trimmed.csv", 50,
            webServer);
#else

//    ByteParser parser("/Users/will/Desktop/test.txt");

//    connectToModule("A28DMVHS", Serving);

    timer = new QTimer();
    timer->setInterval(5);

    loopCount = 0;

    connect(timer, &QTimer::timeout, [this]()
            {
                for (auto radioModule: this->radioModules)
                {
                    radioModule->doCycle();
                }
            }
    );
    timer->start();
#endif
}

Backend::Backend(QObject *parent) : QObject(parent)
{
    loopCount = 0;
}
