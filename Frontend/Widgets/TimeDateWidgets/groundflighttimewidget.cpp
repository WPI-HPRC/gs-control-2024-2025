//
// Created by Nicolas DeBruin on 10/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FlightTimeWidget.h" resolved

#include "groundflighttimewidget.h"
#include "ui_groundflighttimeWidget.h"


GroundFlightTimeWidget::GroundFlightTimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GroundFlightTimeWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::newGroundFlightTime, this, &GroundFlightTimeWidget::newTime);
}

void GroundFlightTimeWidget::newTime(uint32_t newGroundFlightTime)
{
    uint_fast16_t hours = newGroundFlightTime / (60 * 60 * 1000);
    uint_fast8_t minutes = newGroundFlightTime / (60*1000) % 60;
    uint_fast8_t seconds = newGroundFlightTime / 1000 % 60;
    uint_fast16_t milliseconds = newGroundFlightTime % 1000;
    ui->FlightTime->setText(QString::asprintf("%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->FlightTime->setEnabled(true);
}

GroundFlightTimeWidget::~GroundFlightTimeWidget() {
    delete ui;
}
