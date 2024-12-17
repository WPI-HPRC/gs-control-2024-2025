//
// Created by William Scheirey on 12/16/24.
//

#ifndef GS_BACKEND_2024_2025_DATASIMULATIONBUTTON_H
#define GS_BACKEND_2024_2025_DATASIMULATIONBUTTON_H

#include <QPushButton>

class DataSimulationButton: public QPushButton
{
    Q_OBJECT
public:
    explicit DataSimulationButton(QWidget *parent = nullptr);
    bool dataSimulationIsRunning = false;
};


#endif //GS_BACKEND_2024_2025_DATASIMULATIONBUTTON_H
