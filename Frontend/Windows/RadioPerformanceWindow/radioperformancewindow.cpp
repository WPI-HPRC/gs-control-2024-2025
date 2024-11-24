//
// Created by Nicolas DeBruin on 10/26/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RadioPerformanceWindow.h" resolved

#include "radioperformancewindow.h"
#include "ui_RadioPerformanceWindow.h"


RadioPerformanceWindow::RadioPerformanceWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::RadioPerformanceWindow) {
    ui->setupUi(this);
}

RadioPerformanceWindow::~RadioPerformanceWindow() {
    delete ui;
}
