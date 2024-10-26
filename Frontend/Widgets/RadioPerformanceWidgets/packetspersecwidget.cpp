//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PacketsPerSecWidget.h" resolved

#include "packetspersecwidget.h"
#include "ui_PacketsPerSecWidget.h"
#include "Backend/Backend.h"


PacketsPerSecWidget::PacketsPerSecWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PacketsPerSecWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::packetsPerSecond, this, PacketsPerSecWidget::packetsPerSecondUpdate);
}

void PacketsPerSecWidget::packetsPerSecondUpdate(uint32_t packetsPerSec)
{
    ui->Packets->setText(QString::number(packetsPerSec));
    ui->Packets->setEnabled(true);
}

PacketsPerSecWidget::~PacketsPerSecWidget() {
    delete ui;
}
