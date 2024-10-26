//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BytesPerSecWidget.h" resolved

#include "bytespersecwidget.h"
#include "ui_BytesPerSecWidget.h"


BytesPerSecWidget::BytesPerSecWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::BytesPerSecWidget) {
    ui->setupUi(this);
}

BytesPerSecWidget::~BytesPerSecWidget() {
    delete ui;
}
