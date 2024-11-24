//
// Created by Nicolas DeBruin on 10/8/2024.
//

#ifndef GS_FRONTEND_2024_2025_GROUNDFLIGHTTIMEWIDGET_H
#define GS_FRONTEND_2024_2025_GROUNDFLIGHTTIMEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GroundFlightTimeWidget; }
QT_END_NAMESPACE

class GroundFlightTimeWidget : public QWidget {
Q_OBJECT

public:
    explicit GroundFlightTimeWidget(QWidget *parent = nullptr);
    ~GroundFlightTimeWidget() override;

private:
    Ui::GroundFlightTimeWidget *ui;

public slots:
    void newTime(uint32_t newGroundFlightTime);
};


#endif //GS_FRONTEND_2024_2025_GROUNDFLIGHTTIMEWIDGET_H
