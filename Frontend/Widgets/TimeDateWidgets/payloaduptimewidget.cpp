//
// Created by Nicolas DeBruin on 10/8/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PayloadUptimeWidget.h" resolved

#include "payloaduptimewidget.h"
#include "ui_payloadUptimeWidget.h"


PayloadUptimeWidget::PayloadUptimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PayloadUptimeWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, &PayloadUptimeWidget::newPacket);
}

void PayloadUptimeWidget::newPacket(Backend::Telemetry telemPacket)
{
    uint32_t currentUpTime = telemPacket.data.payloadData->p_timestamp;

    emit newUptime(currentUpTime);

    uint_fast16_t hours = currentUpTime / (60 * 60 * 1000);
    uint_fast8_t minutes = currentUpTime / (60*1000) % 60;
    uint_fast8_t seconds = currentUpTime / 1000 % 60;
    uint_fast16_t milliseconds = currentUpTime % 1000;
    ui->PayloadUptime->setText(QString::asprintf("%02d:%02d:%02d.%03d",hours, minutes, seconds, milliseconds));
    ui->PayloadUptime->setEnabled(true);
}

PayloadUptimeWidget::~PayloadUptimeWidget() {
    delete ui;
}
