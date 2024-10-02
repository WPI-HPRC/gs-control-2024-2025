//
// Created by William Scheirey on 8/20/24.
//

#include "Backend.h"
#include <QSerialPort>
#include <QJsonDocument>
#include <string>
#include "Constants.h"
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

void Backend::portOpened(const QSerialPortInfo& portInfo, bool success)
{
    emit(serialPortOpened(portInfo, success));
}

void Backend::portClosed(const QSerialPortInfo& portInfo)
{
    emit(serialPortClosed(portInfo));
}

RadioModule *Backend::getModuleWithName(const QString& name)
{
    RadioModule *module;
    for(int i = 0; i < radioModules.count(); i++)
    {
        if(radioModules.at(i)->serialPort->name().contains(name))
        {
            return radioModules.at(i);
        }
    }
    return nullptr;
}

bool Backend::moduleExistsWithName(const QString &name)
{
    return getModuleWithName(name) != nullptr;
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

void Backend::linkTestFailed()
{
    emit linkTestFailedSignal();
}

void Backend::runLinkTest(uint64_t destinationAddress, uint16_t payloadSize, uint16_t iterations, uint8_t repeat, bool loop)
{
    if(getTargetPort(GROUND_STATION_MODULE).isNull())
        return;

    RadioModule *module = getModuleWithName(GROUND_STATION_MODULE);

    std::cout << "Running link test" << std::endl;
    if(repeat != 0)
    {
        std::cout << "Repeat != 0" << std::endl;
        module->linkTestsLeft = loop ? -1 : repeat;
    }
    else
    {
        std::cout << "Repeat == 0" << std::endl;
        module->linkTestsLeft--;
    }

    std::cout << "LinkTestsLeft = " << module->linkTestsLeft << std::endl;

    module->sendLinkTestRequest(destinationAddress, payloadSize, iterations);
}

void Backend::cancelLinkTest()
{
    std::cout << "Cancelling link test" << std::endl;
    getModuleWithName(GROUND_STATION_MODULE)->linkTestsLeft = 0;
}

void Backend::runThroughputTest(const QString& originatingPort, uint64_t destinationAddress, uint8_t payloadSize,
                                uint packetRate, uint duration, uint8_t transmitOptions)
{
    RadioModule *receiveModule = getModuleWithName(originatingPort);

    if(!receiveModule)
        return;

    throughputTestParams = {
      .receiveModule = receiveModule,
      .destinationAddress = destinationAddress,
      .payloadSize = payloadSize,
      .packetRate = packetRate,
      .duration = duration,
      .transmitOptions = transmitOptions
    };

    throughputTestTimer->setInterval(1000 / (int)packetRate);

    receiveModule->receivingThroughputTest = true;
    receiveModule->throughputTestPacketsReceived = 0;
    receiveModule->logTransmitStatus = false;

    throughputTestStartTime = QDateTime::currentMSecsSinceEpoch();
    throughputTestTimer->start();
};

void Backend::throughputTestTimerTicked()
{
    qint64 currentMs = QDateTime::currentMSecsSinceEpoch();

    if(currentMs - throughputTestStartTime >= throughputTestParams.duration*1000)
    {
        throughputTestTimer->stop();
        this->throughputTestComplete();
        return;
    }

//    std::cout << "Timer ticked. Current ms: " << std::dec << currentMs << std::endl;
    mutex.lock();
    throughputTestParams.receiveModule->sendTransmitRequestCommand(throughputTestParams.destinationAddress, throughputTestParams.transmitOptions,
                                                                   0x00, throughputTestDummyLoad, throughputTestParams.payloadSize);
    mutex.unlock();
}

void Backend::throughputTestComplete()
{
    uint numPacketsReceived = throughputTestParams.receiveModule->throughputTestPacketsReceived;
    uint theoreticalPacketsReceived = throughputTestParams.packetRate * throughputTestParams.duration;

    float percentReceived = (float)numPacketsReceived / (float)theoreticalPacketsReceived * 100;

    float throughput = (float)numPacketsReceived * throughputTestParams.payloadSize / throughputTestParams.duration / 1000 * 8;

    ThroughputTestResults results = {
            .payloadSize = throughputTestParams.payloadSize,
            .packetRate = throughputTestParams.packetRate,
            .duration = throughputTestParams.duration,
            .transmitOptions = throughputTestParams.transmitOptions,
            .numPacketsReceived = numPacketsReceived,
            .percentReceived = percentReceived,
            .throughput = throughput
    };

    std::string str = JS::serializeStruct(results);

    QJsonObject obj = QJsonDocument::fromJson(str.c_str()).object();

    throughputTestParams.receiveModule->dataLogger->logThroughputTest(obj);

    emit throughputTestDataAvailable(percentReceived, numPacketsReceived, throughput);
}

void Backend::sendEnergyDetectCommand(uint16_t msPerChannel)
{
    if(getTargetPort(GROUND_STATION_MODULE).isNull())
        return;

    getModuleWithName(GROUND_STATION_MODULE)->sendEnergyDetectCommand(msPerChannel);
}

void Backend::linkTestComplete(LinkTestResults results, int iterationsLeft)
{
    emit linkTestDataAvailable(results, iterationsLeft);
}

void Backend::disconnectFromModule(const QString &name)
{
    RadioModule *module = getModuleWithName(name);

    if(!module)
        return;

    module->disconnectPort();
}

bool Backend::connectToModule(const QString& name, RadioModuleType moduleType)
{

    RadioModule *existingModule = getModuleWithName(name);
    if(existingModule)
    {
        if(!existingModule->serialPort->isOpen())
        {
            existingModule->connectPort();
        }
        return true;
    }

    QSerialPortInfo targetPort = getTargetPort(name);

    if(targetPort.isNull())
    {
        /*
        qDebug() << "Could not find module " << name;
        QTimer::singleShot(1000, [this, name, moduleType]()
        {
            this->connectToModule(name, moduleType);
        });
        */
        return false;
    }

    RadioModule *module;
    switch(moduleType)
    {
        case Rocket:
            module = new RocketTestModule(921600, new DataLogger(), targetPort);
            break;
        case Payload:
            module = new PayloadTestModule(921600, new DataLogger(), targetPort);
            break;
        default:
            module = new ServingRadioModule(115200, new DataLogger(), targetPort, webServer);
    }
    radioModules.append(module);
    return true;
}

void Backend::runRadioModuleCycles()
{
    mutex.lock();
    for (auto radioModule: this->radioModules)
    {
        radioModule->doCycle();
    }
    mutex.unlock();
}

void Backend::start()
{
    getPorts();

    webServer = new WebServer(8001);

#ifdef SIMULATE_DATA
    webServer = new WebServer(8001);

    dataSimulator = new DataSimulator(
            "/Users/will/Desktop/irec_trimmed.csv", 50,
            webServer);
#else

//    ByteParser parser("/Users/will/Desktop/test.txt");

    QSerialPortInfo modem = getTargetPort(GROUND_STATION_MODULE);
    if(!modem.isNull())
    {
        connectToModule(GROUND_STATION_MODULE, Default);
    }

//    getModuleWithName("A28DMVHS")->sendLinkTestRequest(0x0013A200422CDAC2, 300, 4000);

    timer = new QTimer();
    timer->setInterval(1);

    loopCount = 0;

    connect(timer, &QTimer::timeout, this, &Backend::runRadioModuleCycles);
    timer->start();
#endif
}

Backend::Backend(QObject *parent) : QObject(parent)
{
    loopCount = 0;
    throughputTestTimer = new QTimer();
    connect(throughputTestTimer, &QTimer::timeout, this, &Backend::throughputTestTimerTicked);
}
