//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BytesPerSecWidget.h" resolved

#include "bytespersecwidget.h"
#include "ui_BytesPerSecWidget.h"
#include "Backend/Backend.h"


BytesPerSecWidget::BytesPerSecWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::BytesPerSecWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::bytesPerSecond, this, &BytesPerSecWidget::bytesPerSecondUpdate);
}

void BytesPerSecWidget::bytesPerSecondUpdate(uint64_t bytesPerSec)
{
    ui->Bytes->setText(QString::number(bytesPerSec));
    ui->Bytes->setEnabled(true);
}

BytesPerSecWidget::~BytesPerSecWidget() {
    delete ui;
}
