//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DroppedPacketsWidget.h" resolved

#include "droppedpacketswidget.h"
#include "ui_DroppedPacketsWidget.h"


DroppedPacketsWidget::DroppedPacketsWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DroppedPacketsWidget) {
    ui->setupUi(this);
}

DroppedPacketsWidget::~DroppedPacketsWidget() {
    delete ui;
}
