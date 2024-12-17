//
// Created by William Scheirey on 12/16/24.
//

#include "DataSimulationButton.h"
#include "Backend/Backend.h"

DataSimulationButton::DataSimulationButton(QWidget *parent): QPushButton("Start Data Simulation", parent)
{
    connect(this, &QPushButton::pressed, [this]()
    {
       if(this->dataSimulationIsRunning)
       {
           this->dataSimulationIsRunning = false;
           Backend::getInstance().rocketDataSimulator->stop();
           Backend::getInstance().payloadDataSimulator->stop();
           setText("Start Data Simulation");
       }
       else
       {
           this->dataSimulationIsRunning = true;
           Backend::getInstance().rocketDataSimulator->start();
           Backend::getInstance().payloadDataSimulator->start();
           setText("Stop Data Simulation");
       }
    });
}

