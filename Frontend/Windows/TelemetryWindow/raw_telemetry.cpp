//
// Created by Rafael on 28.09.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Raw_Telemetry.h" resolved

#include "raw_telemetry.h"
#include <QLabel>
#include <QTableWidget>
#include "ui_Raw_Telemetry.h"
#include <QSerialPort>
#include "Backend/Backend.h"

Raw_Telemetry::Raw_Telemetry(QWidget *parent) :
        QWidget(parent), ui(new Ui::Raw_Telemetry)
{
    ui->setupUi(this);
    connect (&Backend::getInstance(), &Backend::telemetryAvailable, this, &Raw_Telemetry::telemetryAvailable);
}
Raw_Telemetry::~Raw_Telemetry() {
    delete ui;
}

void Raw_Telemetry::telemetryAvailable(Backend::Telemetry telemetry){
    if(telemetry.packetType== GroundStation::Rocket)
    {
        HPRC::RocketTelemetryPacket *data = telemetry.data.rocketData;
        processRocketTelemetryData(*data);
    }
    else if(telemetry.packetType== GroundStation::Payload)
    {
        HPRC::PayloadTelemetryPacket *data = telemetry.data.payloadData;
        processPayloadTelemetryData(*data);
    }
    //resizing columns
    ui->Column_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Column_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Column_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Raw_Telemetry::processRocketTelemetryData(const HPRC::RocketTelemetryPacket &data)
{   //Rocket Column1 Update
    updateTable1(0, 1, QString::number(data.accelx()));
    updateTable1(1,1, QString::number(data.accely()));
    updateTable1(2,1, QString::number(data.accelz()));
    updateTable1(3,1,QString::number(data.velx()));
    updateTable1(4,1,QString::number(data.vely()));
    updateTable1(5,1,QString::number(data.velz()));
    updateTable1(6,1,QString::number(data.posx()));
    updateTable1(7,1,QString::number(data.posx()));
    updateTable1(8,1,QString::number(data.posz()));
    updateTable1(9,1,QString::number(data.magx()));
    updateTable1(10,1,QString::number(data.magy()));
    updateTable1(11,1,QString::number(data.magz()));
    updateTable1(12,1,QString::number(data.gyrox()));
    updateTable1(13,1,QString::number(data.gyroy()));
    updateTable1(14,1,QString::number(data.gyroz()));

    //Rocket Column2 update
    updateTable2(0,1,QString::number(data.epochtime()));
    updateTable2(1,1,QString::number(data.timestamp()));
    updateTable2(2,1,QString::number(data.state()));
    //Rocket GPS Lock update
    if (data.gpslock()){
        updateTable2(3,1,"yes");
    }
    else {
        updateTable2(3,1,"no");
    }
    updateTable2(4,1,QString::number(data.satellites()));
    //...

    //Rocket Column3 update
    updateTable3(0,1, QString::number(data.i()));
    updateTable3(1,1, QString::number(data.j()));
    updateTable3(2,1, QString::number(data.k()));
    updateTable3(3,1, QString::number(data.w()));
    updateTable3(4,1,QString::number(data.gpslat()));
    updateTable3(5,1,QString::number(data.gpslong()));
    updateTable3(6,1,QString::number(data.gpsaltmsl()));
    updateTable3(7,1,QString::number(data.gpsaltagl()));
    updateTable3(8,1,QString::number(data.altitude()));
    updateTable3(9,1,QString::number(data.pressure()));
}

void Raw_Telemetry::processPayloadTelemetryData(const HPRC::PayloadTelemetryPacket &data)
{
    //Payload Column1 Update
    updateTable1(0,2,QString::number(data.accelx()));
    updateTable1(1,2,QString::number(data.accely()));
    updateTable1(2,2,QString::number(data.accelz()));
    updateTable1(3,2,QString::number(data.velx()));
    updateTable1(4,2,QString::number(data.vely()));
    updateTable1(5,2,QString::number(data.velz()));
    updateTable1(6,2,QString::number(data.posx()));
    updateTable1(7,2,QString::number(data.posy()));
    updateTable1(8,2,QString::number(data.posz()));
    updateTable1(9,2,QString::number(data.magx()));
    updateTable1(10,2,QString::number(data.magy()));
    updateTable1(11,2,QString::number(data.magz()));
    updateTable1(12,2,QString::number(data.gyrox()));
    updateTable1(13,2,QString::number(data.gyroy()));
    updateTable1(14,2,QString::number(data.gyroz()));

    //Payload Column3 update
    updateTable3(0,2,QString::number(data.i()));
    updateTable3(1,2,QString::number(data.j()));
    updateTable3(2,2,QString::number(data.k()));
    updateTable3(3,2,QString::number(data.w()));
    updateTable3(4,2,QString::number(data.gpslat()));
    updateTable3(5,2,QString::number(data.gpslong()));
    updateTable3(6,2,QString::number(data.gpsaltmsl()));
    updateTable3(7,2,QString::number(data.gpsaltagl()));
    updateTable3(8,2,QString::number(data.altitude()));
    updateTable3(9,2,QString::number(data.pressure()));

    //Payload Column2 update
    updateTable2(0,2,QString::number(data.epochtime()));
    updateTable2(1,2,QString::number(data.timestamp()));
    updateTable2(2,2,QString::number(data.state()));
    //Payload GPS Lock update
    if (data.gpslock()){
        updateTable2(3,2,"yes");
    }
    else
    {
        updateTable2(3,2,"no");
    }
    updateTable2(4,2,QString::number(data.satellites()));
}

void Raw_Telemetry::updateTable1(int row, int column, const QString &value){ //function to update Column 1
    QTableWidget *widget = ui->Column_1;
    auto *cell = (QLabel *)widget->cellWidget(row, column);
    if(!cell)
    {
        widget->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setText(value);
    }
}
void Raw_Telemetry::updateTable2(int row, int column, const QString &value){ //function to update Column 2
    QTableWidget *widget = ui->Column_2;
    auto *cell = (QLabel *)widget->cellWidget(row, column);
    if(!cell)
    {
        widget->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setText(value);
    }
}
void Raw_Telemetry::updateTable3(int row, int column, const QString &value){ //function to update Column 3
    QTableWidget *widget = ui->Column_3;
    auto *cell = (QLabel *)widget->cellWidget(row, column);
    if(!cell)
    {
        widget->setCellWidget(row, column, new QLabel(value));
    }
    else
    {
        cell->setText(value);
    }
}






