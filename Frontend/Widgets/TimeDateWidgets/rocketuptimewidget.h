//
// Created by Nicolas DeBruin on 10/8/2024.
//

#ifndef GS_FRONTEND_2024_2025_ROCKETUPTIMEWIDGET_H
#define GS_FRONTEND_2024_2025_ROCKETUPTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RocketUptimeWidget; }
QT_END_NAMESPACE

class RocketUptimeWidget : public QWidget {
Q_OBJECT

public:
    explicit RocketUptimeWidget(QWidget *parent = nullptr);
    ~RocketUptimeWidget() override;

private:
    Ui::RocketUptimeWidget *ui;

signals:
    void newUptime(uint32_t);

public slots:
    void newPacket(Backend::Telemetry telemPacket);
};


#endif //GS_FRONTEND_2024_2025_ROCKETUPTIMEWIDGET_H
