//
// Created by William Scheirey on 12/16/24.
//

#include "PayloadConnectionIndicator.h"
#include "Backend/Backend.h"

PayloadConnectionIndicator::PayloadConnectionIndicator(QWidget *parent): LedWidget(parent)
{
    this->setLabel("Payload");
    connect(&Backend::getInstance(), &Backend::telemetryAvailable, this, [this](Backend::Telemetry telemetry)
    {
        if(telemetry.packetType == GroundStation::Payload)
        {
            this->resetTimer();
        }
    });
}