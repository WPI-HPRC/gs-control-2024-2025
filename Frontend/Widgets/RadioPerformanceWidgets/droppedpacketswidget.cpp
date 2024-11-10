//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DroppedPacketsWidget.h" resolved

#include "droppedpacketswidget.h"
#include "ui_DroppedPacketsWidget.h"
#include "Backend/Backend.h"


DroppedPacketsWidget::DroppedPacketsWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DroppedPacketsWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::droppedPackets, this, &DroppedPacketsWidget::droppedPacketsCount);
}

void DroppedPacketsWidget::droppedPacketsCount(uint32_t count)
{
    ui->DroppedPackets->setText(QString::number(count));
    ui->DroppedPackets->setEnabled(true);
}

DroppedPacketsWidget::~DroppedPacketsWidget() {
    delete ui;
}
