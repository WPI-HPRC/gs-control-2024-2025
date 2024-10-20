//
// Created by Nicolas DeBruin on 10/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightTimeWidget.h" resolved

#include "rocketflighttimewidget.h"
#include "ui_rocketflighttimeWidget.h"


RocketFlightTimeWidget::RocketFlightTimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RocketFlightTimeWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::newRocketFlightTime, this, &RocketFlightTimeWidget::newTime);
}

void RocketFlightTimeWidget::newTime(uint32_t newRocketFlightTime)
{
    uint_fast16_t hours = newRocketFlightTime / (60 * 60 * 1000);
    uint_fast8_t minutes = newRocketFlightTime / (60*1000) % 60;
    uint_fast8_t seconds = newRocketFlightTime / 1000 % 60;
    uint_fast16_t milliseconds = newRocketFlightTime % 1000;
    ui->FlightTime->setText(QString::asprintf("%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->FlightTime->setEnabled(true);
}

RocketFlightTimeWidget::~RocketFlightTimeWidget() {
    delete ui;
}
