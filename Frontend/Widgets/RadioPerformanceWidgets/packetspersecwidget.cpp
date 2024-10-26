//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PacketsPerSecWidget.h" resolved

#include "packetspersecwidget.h"
#include "ui_PacketsPerSecWidget.h"


PacketsPerSecWidget::PacketsPerSecWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PacketsPerSecWidget) {
    ui->setupUi(this);
}

PacketsPerSecWidget::~PacketsPerSecWidget() {
    delete ui;
}
