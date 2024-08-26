//
// Created by William Scheirey on 8/24/24.
//

#include "SerialPortList.h"
#include "Backend/Backend.h"
#include <iostream>

SerialPortList::SerialPortList(QWidget *parent) : QListWidget(parent)
{
    Backend &backend = Backend::getInstance();

    connect(&backend, SIGNAL(foundSerialPorts(QList<QSerialPortInfo>)), this, SLOT(serialPortsFound(QList<QSerialPortInfo>)));
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(portChosen(QListWidgetItem*)));
}

void SerialPortList::serialPortsFound(const QList<QSerialPortInfo>& ports)
{
    for(const QSerialPortInfo& port : ports)
    {
        if(port.portName().contains("Bluetooth") || port.portName().contains("debug-console"))
        {
            continue;
        }

        this->serialPorts.append(port);
        std::cout << port.portName().toStdString() << std::endl;
        this->addItem(port.portName());
    }
}

void SerialPortList::portChosen(QListWidgetItem *item)
{
    int rowIndex = indexFromItem(item).row();
    QString portName = serialPorts.at(rowIndex).portName();
    Backend::getInstance().connectToModule(portName, Serving);
}
