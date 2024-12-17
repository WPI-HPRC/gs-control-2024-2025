//
// Created by Nicolas DeBruin on 12/4/2024.
//

#ifndef THROUGHPUTDISPLAYWIDGET_H
#define THROUGHPUTDISPLAYWIDGET_H

#include <QWidget>
#include "Backend/RadioModule.h"
#include "Backend/Backend.h"
#include "Utility/Utility.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ThroughputDisplayWidget; }
QT_END_NAMESPACE

class ThroughputDisplayWidget : public QWidget {
Q_OBJECT

public:
    explicit ThroughputDisplayWidget(QWidget *parent = nullptr);
    ~ThroughputDisplayWidget() override;

    void chooseTarget(RadioPerformanceStat::PacketType target);

    void chooseType(RadioPerformanceStat::StatType stat, RadioPerformanceStat::DataType dataType);

public slots:
        void throughPutStatsUpdate(RadioThroughputStats);
        void countStatsUpdate(RadioCountStats);

private:
    Ui::ThroughputDisplayWidget *ui;

    QMetaObject::Connection connection;

    RadioPerformanceStat::PacketType currentTarget = RadioPerformanceStat::PacketType::NoPacketType;
    RadioPerformanceStat::DataType currentDataType = RadioPerformanceStat::DataType::NoDataType;
    RadioPerformanceStat::StatType currentStatType = RadioPerformanceStat::StatType::NoStatType;
};


#endif //THROUGHPUTDISPLAYWIDGET_H
