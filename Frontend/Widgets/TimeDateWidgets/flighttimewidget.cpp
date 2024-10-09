//
// Created by Nicolas DeBruin on 10/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightTimeWidget.h" resolved

#include "flighttimewidget.h"
#include "ui_flighttimeWidget.h"


FlightTimeWidget::FlightTimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::FlightTimeWidget) {
    ui->setupUi(this);

    //connect(&Backend::getInstance(), &Backend::newFlightTime, this, &FlightTimeWidget::newTime);
}

void FlightTimeWidget::newPacket(Backend::Telemetry telemPacket)
{


    // uint_fast16_t hours = currentFlightTime / (60 * 60 * 1000);
    // uint_fast8_t minutes = currentFlightTime / (60*1000) % 60;
    // uint_fast8_t seconds = currentFlightTime / 1000 % 60;
    // uint_fast16_t milliseconds = currentFlightTime % 1000;
    // ui->FlightTime->setText(QString::asprintf("%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->FlightTime->setEnabled(true);
}

FlightTimeWidget::~FlightTimeWidget() {
    delete ui;
}
