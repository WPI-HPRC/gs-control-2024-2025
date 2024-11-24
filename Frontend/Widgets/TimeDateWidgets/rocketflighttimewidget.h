//
// Created by Nicolas DeBruin on 10/8/2024.
//

#ifndef GS_FRONTEND_2024_2025_ROCKETFLIGHTTIMEWIDGET_H
#define GS_FRONTEND_2024_2025_ROCKETFLIGHTTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RocketFlightTimeWidget; }
QT_END_NAMESPACE

class RocketFlightTimeWidget : public QWidget {
Q_OBJECT

public:
    explicit RocketFlightTimeWidget(QWidget *parent = nullptr);
    ~RocketFlightTimeWidget() override;

private:
    Ui::RocketFlightTimeWidget *ui;

public slots:
    void newTime(uint32_t newRocketFlightTime);
};


#endif //GS_FRONTEND_2024_2025_ROCKETFLIGHTTIMEWIDGET_H
