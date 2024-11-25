//
// Created by Rafael on 28.09.24.
//

#ifndef RAW_TELEMETRY_H
#define RAW_TELEMETRY_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QSerialPort>

#include "Backend/Backend.h"
#include <QWidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Raw_Telemetry; }
QT_END_NAMESPACE

class Raw_Telemetry : public QWidget
{
Q_OBJECT

public:
    explicit Raw_Telemetry(QWidget *parent = nullptr);
    ~Raw_Telemetry() override;

private slots:
    void telemetryAvailable(Backend::Telemetry telemetry);

private:
    Ui::Raw_Telemetry *ui;
    QSerialPort *serialPort;
    void updateTable1(int row, int column, const QString &value); //function to update Column 1 cells
    void updateTable2(int row, int column, const QString &value); //function to update Column 2 cells
    void updateTable3(int row, int column, const QString &value); //function to update Column 3 cells
    void processRocketTelemetryData(const GroundStation::RocketTelemPacket &data); //function to process the incoming telemetry data
    void processPayloadTelemetryData(const GroundStation::PayloadTelemPacket &data); //function to process the incoming telemetry data
};

#endif //RAW_TELEMETRY_H
