//
// Created by William Scheirey on 8/24/24.
//

#ifndef GS_BACKEND_2024_2025_SERIALPORTLIST_H
#define GS_BACKEND_2024_2025_SERIALPORTLIST_H

#include <QListWidget>
#include <QSerialPortInfo>

class SerialPortList: public QListWidget
{
    Q_OBJECT
public:
    explicit SerialPortList(QWidget *parent = nullptr);

public slots:
    void serialPortsFound(const QList<QSerialPortInfo>&);
    void portChosen(QListWidgetItem *);

private:
    QList<QSerialPortInfo> serialPorts;
};


#endif //GS_BACKEND_2024_2025_SERIALPORTLIST_H
