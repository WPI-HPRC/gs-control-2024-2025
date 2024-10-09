//
// Created by Rafael on 28.09.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Raw_Telemetry.h" resolved

#include "raw_telemetry.h"
#include <QLabel>
#include <QTableWidget>
#include "ui_Raw_Telemetry.h"


Raw_Telemetry::Raw_Telemetry(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Raw_Telemetry) {
    ui->setupUi(this);

    int myInt = 150;

    auto *label = new QLabel(QString::asprintf("%d", myInt));
    QTableWidget *widget = ui->Column_1;
    widget->setCellWidget(1, 1, label);
}

Raw_Telemetry::~Raw_Telemetry() {
    delete ui;
}
