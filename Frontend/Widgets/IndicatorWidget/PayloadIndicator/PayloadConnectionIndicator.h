//
// Created by William Scheirey on 12/16/24.
//

#ifndef GS_BACKEND_2024_2025_PAYLOADCONNECTIONINDICATOR_H
#define GS_BACKEND_2024_2025_PAYLOADCONNECTIONINDICATOR_H

#include "Frontend/Widgets/IndicatorWidget/IndicatorWidget/ledwidget.h"

class PayloadConnectionIndicator: public LedWidget
{
    Q_OBJECT
    public:
        explicit PayloadConnectionIndicator(QWidget *parent = nullptr);
};


#endif //GS_BACKEND_2024_2025_PAYLOADCONNECTIONINDICATOR_H
