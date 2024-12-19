//
// Created by Nicolas DeBruin on 12/4/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ThroughputDisplayWidget.h" resolved

#include "throughputdisplaywidget.h"
#include "ui_ThroughputDisplayWidget.h"


ThroughputDisplayWidget::ThroughputDisplayWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ThroughputDisplayWidget) {
    ui->setupUi(this);
}

void ThroughputDisplayWidget::throughPutStatsUpdate(RadioThroughputStats stats)
{
    switch(currentDataType)
    {
        case RadioPerformanceStat::DataType::Packets:
            ui->Data->setText(QString::number(stats.packetsPerSecond));
            break;
        case RadioPerformanceStat::DataType::Bytes:
            ui->Data->setText(QString::number(stats.bytesPerSecond));
            break;
        default:
            ui->Data->setText("");
            break;
    }
    ui->Data->setEnabled(true);
}

void ThroughputDisplayWidget::countStatsUpdate(RadioCountStats stats)
{
    switch(currentDataType)
    {
        case RadioPerformanceStat::DataType::Packets:
            ui->Data->setText(QString::number(stats.packetsReceivedCount));
            break;
        case RadioPerformanceStat::DataType::Bytes:
            ui->Data->setText(QString::number(stats.bytesReceivedCount));
            break;
        default:
            ui->Data->setText("");
            break;
    }
    ui->Data->setEnabled(true);
}

void ThroughputDisplayWidget::chooseType(RadioPerformanceStat::StatType stat, RadioPerformanceStat::DataType dataType)
{
    currentStatType = stat;
    currentDataType = dataType;

    switch(currentStatType)
    {
        case RadioPerformanceStat::StatType::Throughput:
        {
            ui->DataLabel->setText("Throughput:");
            switch (currentDataType)
            {
                case RadioPerformanceStat::DataType::Packets:
                    ui->DataUnit->setText("Packets/s");
                    break;
                case RadioPerformanceStat::DataType::Bytes:
                    ui->DataUnit->setText("Bytes/s");
                    break;
                default:
                    ui->DataUnit->setText("");
                    break;
            }
            break;
        }
        case RadioPerformanceStat::StatType::Count:
        {
            ui->DataLabel->setText("Total Received Count:");
            switch (currentDataType)
            {
                case RadioPerformanceStat::DataType::Packets:
                    ui->DataUnit->setText("Packets");
                    break;
                case RadioPerformanceStat::DataType::Bytes:
                    ui->DataUnit->setText("Bytes");
                    break;
                default:
                    ui->DataUnit->setText("");
                    break;
            }
            break;
        }
        default:
        {
            ui->DataLabel->setText("");
            ui->DataUnit->setText("");
            break;
        }
    }
}

void ThroughputDisplayWidget::chooseTarget(RadioPerformanceStat::PacketType target)
{
    disconnect(connection); // disconnect the previous thing
    switch (target)
    {
        case RadioPerformanceStat::PacketType::Rocket:
        {
            currentTarget = RadioPerformanceStat::PacketType::Rocket;
            switch (currentStatType)
            {
                case RadioPerformanceStat::StatType::Count:
                    connection = connect(&Backend::getInstance(), &Backend::rocketCountStats, this,
                                         &ThroughputDisplayWidget::countStatsUpdate);
                    break;
                case RadioPerformanceStat::StatType::Throughput:
                    connection = connect(&Backend::getInstance(), &Backend::rocketThroughputStats, this,
                                         &ThroughputDisplayWidget::throughPutStatsUpdate);
                    break;
                default:
                    break;
            }
            break;
        }
        case RadioPerformanceStat::PacketType::Payload:
        {
            currentTarget = RadioPerformanceStat::PacketType::Payload;
            disconnect(connection);
            switch (currentStatType)
            {
                case RadioPerformanceStat::StatType::Count:
                    connection = connect(&Backend::getInstance(), &Backend::payloadCountStats, this,
                                         &ThroughputDisplayWidget::countStatsUpdate);
                    break;
                case RadioPerformanceStat::StatType::Throughput:
                    connection = connect(&Backend::getInstance(), &Backend::payloadThroughputStats, this,
                                         &ThroughputDisplayWidget::throughPutStatsUpdate);
                    break;
                default:
                    break;;
            }
            break;
        }
        case RadioPerformanceStat::PacketType::Combined:
        {
            currentTarget = RadioPerformanceStat::PacketType::Combined;
            switch (currentStatType)
            {
                case RadioPerformanceStat::StatType::Count:
                    connection = connect(&Backend::getInstance(), &Backend::combinedCountStats, this,
                                         &ThroughputDisplayWidget::countStatsUpdate);
                    break;
                case RadioPerformanceStat::StatType::Throughput:
                    connection = connect(&Backend::getInstance(), &Backend::combinedThroughputStats, this,
                                         &ThroughputDisplayWidget::throughPutStatsUpdate);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

ThroughputDisplayWidget::~ThroughputDisplayWidget() {
    delete ui;
}
