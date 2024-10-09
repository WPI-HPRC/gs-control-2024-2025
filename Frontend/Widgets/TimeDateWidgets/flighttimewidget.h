//
// Created by Nicolas DeBruin on 10/8/2024.
//

#ifndef GS_FRONTEND_2024_2025_FLIGHTTIMEWIDGET_H
#define GS_FRONTEND_2024_2025_FLIGHTTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FlightTimeWidget; }
QT_END_NAMESPACE

class FlightTimeWidget : public QWidget {
Q_OBJECT

public:
    explicit FlightTimeWidget(QWidget *parent = nullptr);
    ~FlightTimeWidget() override;

private:
    Ui::FlightTimeWidget *ui;

signals:
    void newTime(uint32_t);

public slots:
    void newPacket(Backend::Telemetry telemPacket);
};


#endif //GS_FRONTEND_2024_2025_FLIGHTTIMEWIDGET_H
