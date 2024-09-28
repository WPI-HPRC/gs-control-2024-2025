//
// Created by William Scheirey on 8/24/24.
//

#include "SerialPortList.h"
#include "Backend/Backend.h"
#include <iostream>
#include <QHeaderView>
#include <QMainWindow>
#include <qapplication.h>
#include <QComboBox>

QMainWindow* getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (auto* mainWin = qobject_cast<QMainWindow*>(w))
            return mainWin;

    return nullptr;
}

SerialPortList::SerialPortList(QWidget *parent) : QTableWidget(parent)
{
    Backend &backend = Backend::getInstance();

    connect(&backend, SIGNAL(foundSerialPorts(QList<QSerialPortInfo>)), this, SLOT(serialPortsFound(QList<QSerialPortInfo>)));

    connect(&backend, SIGNAL(serialPortOpened(QSerialPortInfo, bool)), this, SLOT(serialPortOpened(QSerialPortInfo, bool)));
    connect(&backend, SIGNAL(serialPortClosed(QSerialPortInfo)), this, SLOT(serialPortClosed(QSerialPortInfo)));

//    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(portChosen(QListWidgetItem*)));

    connect(this, &SerialPortList::openSerialPort, &SerialPortManager::getInstance(), &SerialPortManager::openPort);
    connect(this, SIGNAL(closeSerialPort(QString)), &SerialPortManager::getInstance(), SLOT(closePort(QString)));

    /*
    thread = new QThread;
    SerialPortManager::getInstance().moveToThread(thread);
    thread->start();
     */
}

void SerialPortList::serialPortsFound(const QList<QSerialPortInfo>& ports)
{
    this->serialPorts.clear();
    for(const QSerialPortInfo& port : ports)
    {
        if(port.portName().contains("Bluetooth") || port.portName().contains("debug-console"))
        {
            continue;
        }
        this->serialPorts.append(port);
    }

    this->setColumnCount(2);
    this->setRowCount((int)this->serialPorts.count());

    for(int i = 0; i < this->serialPorts.count(); i++)
    {
        auto *button = new QPushButton(Backend::getInstance().moduleExistsWithName(serialPorts.at(i).portName()) ? "Disconnect" : "Connect");
        button->setProperty("row", i);
        button->setStyleSheet("margin: 0 10px 0 10px");
        button->setObjectName(this->serialPorts.at(i).portName());

        connect(button, &QPushButton::clicked, this, &SerialPortList::buttonClicked);

        auto *label = new QLabel(this->serialPorts.at(i).portName());
        label->setIndent(10);

        this->setCellWidget(i, 0, label);
        this->setCellWidget(i, 1, button);
    }
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QString SerialPortList::getCurrentlySelectedPortName()
{
    if(this->selectedIndexes().count() == 0)
        return "";
    int index = this->selectedIndexes().at(0).row();
    return serialPorts.at(index).portName();
}

void SerialPortList::buttonClicked()
{
    auto *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();

    if(row >= serialPorts.length())
        return;
    QString portName = serialPorts.at(row).portName();

    if(button->text() == "Connect")
    {
        button->setText("Connecting...");
        button->setEnabled(false);

        int baud = parent()->findChild<QComboBox *>("BaudRateDropdown")->currentText().toInt();

        emit(openSerialPort(portName, Backend::Default, baud));
    }
    else
    {
        button->setText("Disconnecting...");
        button->setEnabled(false);

        emit(closeSerialPort(portName));
    }
}


void SerialPortList::serialPortOpened(const QSerialPortInfo& info, bool success)
{
    qDebug() << "Serial port opened: " << info.portName();

    auto *button = this->findChild<QPushButton *>(info.portName());

    if(!button)
        return;

    button->setText(success ? "Disconnect" : "Connect");
    button->setEnabled(true);
}

void SerialPortList::serialPortClosed(const QSerialPortInfo& info)
{
    qDebug() << "Serial port closed: " << info.portName();

    auto *button = this->findChild<QPushButton *>(info.portName());

    button->setText("Connect");
    button->setEnabled(true);
}


/*
void SerialPortList::portChosen(QListWidgetItem *item)
{
    int rowIndex = indexFromItem(item).row();
    if(rowIndex >= serialPorts.length())
        return;
    QString portName = serialPorts.at(rowIndex).portName();
    Backend::getInstance().connectToModule(portName, Serving);
}
 */
