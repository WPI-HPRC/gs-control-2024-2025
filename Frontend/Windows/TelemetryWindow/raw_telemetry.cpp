//
// Created by Rafael on 28.09.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Raw_Telemetry.h" resolved

#include "raw_telemetry.h"
#include "ui_Raw_Telemetry.h"


Raw_Telemetry::Raw_Telemetry(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Raw_Telemetry) {
    ui->setupUi(this);
}

Raw_Telemetry::~Raw_Telemetry() {
    delete ui;
}
