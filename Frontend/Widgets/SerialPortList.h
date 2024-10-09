//
// Created by William Scheirey on 8/24/24.
//

#ifndef GS_BACKEND_2024_2025_SERIALPORTLIST_H
#define GS_BACKEND_2024_2025_SERIALPORTLIST_H

#include <QTableWidget>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QLabel>
#include <QThread>
#include <Utility/SerialPortManager.h>

class SerialPortList: public QTableWidget
{
    Q_OBJECT
public:
    explicit SerialPortList(QWidget *parent = nullptr);
    QString getCurrentlySelectedPortName();

public slots:
    void serialPortsFound(const QList<QSerialPortInfo>&);
    void buttonClicked();
    void serialPortOpened(const QSerialPortInfo&, bool);
    void serialPortClosed(const QSerialPortInfo&);

signals:
    void openSerialPort(const QString&, Backend::RadioModuleType);
    void closeSerialPort(const QString&);

private:
    QList<QSerialPortInfo> serialPorts;
};


#endif //GS_BACKEND_2024_2025_SERIALPORTLIST_H
