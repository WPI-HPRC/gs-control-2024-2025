//
// Created by Nicolas DeBruin on 10/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_rocketUptimeWidget.h" resolved

#include "rocketuptimewidget.h"
#include "ui_rocketUptimeWidget.h"


RocketUptimeWidget::RocketUptimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RocketUptimeWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &RocketUptimeWidget::newPacket);
}

void RocketUptimeWidget::newPacket(Backend::Telemetry telemPacket)
{
    uint_fast32_t currentUpTime = telemPacket.data.rocketData->timestamp();

    emit newUptime(currentUpTime);

    uint_fast16_t hours = currentUpTime / (60 * 60 * 1000);
    uint_fast8_t minutes = currentUpTime / (60*1000) % 60;
    uint_fast8_t seconds = currentUpTime / 1000 % 60;
    uint_fast16_t milliseconds = currentUpTime % 1000;
    ui->RocketUptime->setText(QString::asprintf("%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->RocketUptime->setEnabled(true);
}

RocketUptimeWidget::~RocketUptimeWidget() {
    delete ui;
}
