//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RadioPerformanceWindow.h" resolved

#include "radioperformancewindow.h"
#include "ui_RadioPerformanceWindow.h"


RadioPerformanceWindow::RadioPerformanceWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::RadioPerformanceWindow) {
    ui->setupUi(this);

    ui->RocketByteCount->chooseType(RadioPerformanceStat::StatType::Count, RadioPerformanceStat::DataType::Bytes);
    ui->RocketByteCount->chooseTarget(RadioPerformanceStat::PacketType::Rocket);

    ui->RocketByteSpeed->chooseType(RadioPerformanceStat::StatType::Throughput, RadioPerformanceStat::DataType::Bytes);
    ui->RocketByteSpeed->chooseTarget(RadioPerformanceStat::PacketType::Rocket);

    ui->RocketPacketCount->chooseType(RadioPerformanceStat::StatType::Count, RadioPerformanceStat::DataType::Packets);
    ui->RocketPacketCount->chooseTarget(RadioPerformanceStat::PacketType::Rocket);

    ui->RocketPacketSpeed->chooseType(RadioPerformanceStat::StatType::Throughput, RadioPerformanceStat::DataType::Packets);
    ui->RocketPacketSpeed->chooseTarget(RadioPerformanceStat::PacketType::Rocket);

    ui->PayloadByteCount->chooseType(RadioPerformanceStat::StatType::Count, RadioPerformanceStat::DataType::Bytes);
    ui->PayloadByteCount->chooseTarget(RadioPerformanceStat::PacketType::Payload);

    ui->PayloadByteSpeed->chooseType(RadioPerformanceStat::StatType::Throughput, RadioPerformanceStat::DataType::Bytes);
    ui->PayloadByteSpeed->chooseTarget(RadioPerformanceStat::PacketType::Payload);

    ui->PayloadPacketCount->chooseType(RadioPerformanceStat::StatType::Count, RadioPerformanceStat::DataType::Packets);
    ui->PayloadPacketCount->chooseTarget(RadioPerformanceStat::PacketType::Payload);

    ui->PayloadPacketSpeed->chooseType(RadioPerformanceStat::StatType::Throughput, RadioPerformanceStat::DataType::Packets);
    ui->PayloadPacketSpeed->chooseTarget(RadioPerformanceStat::PacketType::Payload);
}

RadioPerformanceWindow::~RadioPerformanceWindow() {
    delete ui;
}
