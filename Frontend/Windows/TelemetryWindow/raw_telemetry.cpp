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
    QMainWindow(parent), ui(new Ui::Raw_Telemetry)
{
    ui->setupUi(this);
    connect (&Backend::getInstance(), &Backend::telemetryAvailable, this, Raw_Telemetry::telemetryAvailable);
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
{
    updateTable(1, 1, QString::number(data.accelX));
    /*
    QList<QByteArray> telemetryValues = data.split(',');
    if(telemetryValues.size() >= 30)
    {
        QVector<QPair<int, int>> tableMapping = {
            {0,1},
            {1,1},
            {2,1},
            {3,1},
            {4,1},
            {5,1},
            {6,1},
            {7,1},
            {8,1},
            {9,1},
            {10,1},
            {11,1},
            {12,1},
            {13,1},
            {14,1},
            {0,2},
            {1,2},
            {2,2},
            {3,2},
            {4,2},
            {5,2},
            {6,2},
            {7,2},
            {8,2},
            {9,2},
            {10,2},
            {11,2},
            {12,2},
            {13,2},
            {14,2},
        };
        //Loop through the mapping and update the table
        for (int i=0; i<tableMapping.size(); i++){
            int row = tableMapping[i].first;
            int column = tableMapping[i].second;
            QString value = telemetryValues.at(i);
            updateTable(row,column,value);
        }
    }
    */
}


void Raw_Telemetry::updateTable(int row, int column, const QString &value){
    QTableWidget *widget = ui->Column_1;
    auto *label = new QLabel(value);
    widget->setCellWidget(row, column, label);
}


