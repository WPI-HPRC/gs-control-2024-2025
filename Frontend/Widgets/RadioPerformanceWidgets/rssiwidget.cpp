//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LastPacketdBm.h" resolved

#include "rssiwidget.h"
#include "rssiwidget.h"
#include "ui_RSSIWidget.h"
#include "Backend/Backend.h"


RSSIWidget::RSSIWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RSSIWidget) {
    ui->setupUi(this);

    connect(&Backend::getInstance(), &Backend::receivedAtCommandResponse, this, &RSSIWidget::receiveAtCommandResponse);
}

void RSSIWidget::receiveAtCommandResponse(uint16_t command, const uint8_t *response, size_t response_length_bytes)
{
    if(command == XBee::AtCommand::LastPacketRSSI)
    {
        ui->RSSI->setText("-" + QString::number(response[0]));
        ui->RSSI->setEnabled(true);
    }
}

RSSIWidget::~RSSIWidget() {
    delete ui;
}
