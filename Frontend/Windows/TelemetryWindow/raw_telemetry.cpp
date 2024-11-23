// Created by Rafael Mirzoyan on 28.09.24.

// You may need to build the project (run Qt uic code generator) to get "ui_Raw_Telemetry.h" resolved

#include "raw_telemetry.h"
#include <QLabel>
#include <QTableWidget>
#include "ui_Raw_Telemetry.h"
#include <QSerialPort>
#include "Backend/Backend.h"

Raw_Telemetry::Raw_Telemetry(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Raw_Telemetry)
{
    ui->setupUi(this);
    connect (&Backend::getInstance(), &Backend::telemetryAvailable, this, &Raw_Telemetry::telemetryAvailable);
}
Raw_Telemetry::~Raw_Telemetry() {
    delete ui;
}

void Raw_Telemetry::telemetryAvailable(Backend::Telemetry telemetry){
    if(telemetry.packetType== GroundStation::Rocket){
    GroundStation::RocketTelemPacket *data = telemetry.data.rocketData;
        processTelemetryData(*data);
    }
    else if(telemetry.packetType== GroundStation::Payload){
    GroundStation::PayloadTelemPacket *data = telemetry.data.payloadData;}
}

void Raw_Telemetry::processTelemetryData(const GroundStation::RocketTelemPacket &data)
{   //Rocket Column1 Update
    updateTable1(0, 1, QString::number(data.accelX));
    updateTable1(1,1, QString::number(data.accelY));
    updateTable1(2,1, QString::number(data.accelZ));
    updateTable1(3,1,QString::number(data.velX));
    updateTable1(4,1,QString::number(data.velY));
    updateTable1(5,1,QString::number(data.velZ));
    updateTable1(6,1,QString::number(data.posX));
    updateTable1(7,1,QString::number(data.posY));
    updateTable1(8,1,QString::number(data.posZ));
    updateTable1(9,1,QString::number(data.magX));
    updateTable1(10,1,QString::number(data.magY));
    updateTable1(11,1,QString::number(data.magZ));
    updateTable1(12,1,QString::number(data.gyroX));
    updateTable1(13,1,QString::number(data.gyroY));
    updateTable1(14,1,QString::number(data.gyroZ));
    //Payload Column1 Update
    updateTable1(0,2,QString::number(data.accelX));
    updateTable1(1,2,QString::number(data.accelY));
    updateTable1(2,2,QString::number(data.accelZ));
    updateTable1(3,2,QString::number(data.velX));
    updateTable1(4,2,QString::number(data.velY));
    updateTable1(5,2,QString::number(data.velZ));
    updateTable1(6,2,QString::number(data.posX));
    updateTable1(7,2,QString::number(data.posY));
    updateTable1(8,2,QString::number(data.posZ));
    updateTable1(9,2,QString::number(data.magX));
    updateTable1(10,2,QString::number(data.magY));
    updateTable1(11,2,QString::number(data.magZ));
    updateTable1(12,2,QString::number(data.gyroX));
    updateTable1(13,2,QString::number(data.gyroY));
    updateTable1(14,2,QString::number(data.gyroZ));

    //Rocket Column2 update
    updateTable2(0,1,QString::number(data.epochTime));
    updateTable2(1,1,QString::number(data.timestamp));
    updateTable2(2,1,QString::number(data.state));
    //Rocket GPS Lock update
    if (data.gpsLock){
        updateTable2(3,1,"yes");
    }
    else {
        updateTable2(3,1,"no");
    }
    updateTable2(4,1,QString::number(data.satellites));
    //...

    //Payload Column2 update
    updateTable2(0,2,QString::number(data.epochTime));
    updateTable2(1,2,QString::number(data.timestamp));
    updateTable2(2,2,QString::number(data.state));
    //Payload GPS Lock update
    if (data.gpsLock){
        updateTable2(3,2,"yes");
    }
    else
    {
        updateTable2(3,2,"no");
    }
    updateTable2(4,2,QString::number(data.satellites));
    //...


    //Rocket Column3 update
    updateTable3(0,1, QString::number(data.i));
    updateTable3(1,1, QString::number(data.j));
    updateTable3(2,1, QString::number(data.k));
    updateTable3(3,1, QString::number(data.w));
    updateTable3(4,1,QString::number(data.gpsLat));
    updateTable3(5,1,QString::number(data.gpsLong));
    updateTable3(6,1,QString::number(data.gpsAltMSL));
    updateTable3(7,1,QString::number(data.gpsAltAGL));
    updateTable3(8,1,QString::number(data.altitude));
    updateTable3(9,1,QString::number(data.pressure));
    //Payload Column3 update
    updateTable3(0,2,QString::number(data.i));
    updateTable3(1,2,QString::number(data.j));
    updateTable3(2,2,QString::number(data.k));
    updateTable3(3,2,QString::number(data.w));
    updateTable3(4,2,QString::number(data.gpsLat));
    updateTable3(5,2,QString::number(data.gpsLong));
    updateTable3(6,2,QString::number(data.gpsAltMSL));
    updateTable3(7,2,QString::number(data.gpsAltAGL));
    updateTable3(8,2,QString::number(data.altitude));
    updateTable3(9,2,QString::number(data.pressure));

    //resizing columns
    ui->Column_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Column_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void Raw_Telemetry::updateTable1(int row, int column, const QString &value){ //function to update Column 1
    QTableWidget *widget = ui->Column_1;
    auto *label = new QLabel(value);
    widget->setCellWidget(row, column, label);
}
void Raw_Telemetry::updateTable2(int row, int column, const QString &value){ //function to update Column 2
    QTableWidget *widget = ui->Column_2;
    auto *label = new QLabel(value);
    widget->setCellWidget(row, column, label);
}
void Raw_Telemetry::updateTable3(int row, int column, const QString &value){ //function to update Column 3
    QTableWidget *widget = ui->Column_3;
    auto *label = new QLabel(value);
    widget->setCellWidget(row, column, label);
}






