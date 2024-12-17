//
// Created by William Scheirey on 12/16/24.
//

#ifndef GS_BACKEND_2024_2025_ROCKETCONNECTIONINDICATOR_H
#define GS_BACKEND_2024_2025_ROCKETCONNECTIONINDICATOR_H

#include "Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h"

class RocketConnectionIndicator: public LedWidget
{
    Q_OBJECT
public:
    explicit RocketConnectionIndicator(QWidget *parent = nullptr);
};


#endif //GS_BACKEND_2024_2025_ROCKETCONNECTIONINDICATOR_H
