//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LastPacketdBm.h" resolved

#include "rssiwidget.h"
#include "ui_LastPacketdBm.h"


LastPacketdBm::LastPacketdBm(QWidget *parent) :
    QWidget(parent), ui(new Ui::LastPacketdBm) {
    ui->setupUi(this);
}

LastPacketdBm::~LastPacketdBm() {
    delete ui;
}
