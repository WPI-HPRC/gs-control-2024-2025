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

QT_BEGIN_NAMESPACE
namespace Ui { class Raw_Telemetry; }
QT_END_NAMESPACE

class Raw_Telemetry : public QMainWindow {
Q_OBJECT

public:
    explicit Raw_Telemetry(QWidget *parent = nullptr);
    ~Raw_Telemetry() override;

private slots:
    void telemetryAvailable(Backend::Telemetry telemetry);

private:
    Ui::Raw_Telemetry *ui;
    QSerialPort *serialPort;
    void updateTable(int row, int column, const QString &value); //function to update table cells
    void processTelemetryData(const GroundStation::RocketTelemPacket &data); //function to process the incoming telemetry data
};

#endif //RAW_TELEMETRY_H
