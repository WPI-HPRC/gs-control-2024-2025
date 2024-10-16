//
// Created by Nicolas DeBruin on 10/8/2024.
//

#ifndef GS_FRONTEND_2024_2025_PAYLOADUPTIMEWIDGET_H
#define GS_FRONTEND_2024_2025_PAYLOADUPTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PayloadUptimeWidget; }
QT_END_NAMESPACE

class PayloadUptimeWidget : public QWidget {
Q_OBJECT

public:
    explicit PayloadUptimeWidget(QWidget *parent = nullptr);
    ~PayloadUptimeWidget() override;

private:
    Ui::PayloadUptimeWidget *ui;

signals:
    void newUptime(uint32_t);

public slots:
    void newPacket(Backend::Telemetry telemPacket);
};


#endif //GS_FRONTEND_2024_2025_PAYLOADUPTIMEWIDGET_H
