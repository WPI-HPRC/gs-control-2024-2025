//
// Created by Nicolas DeBruin on 10/3/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimeTestWindow.h" resolved

#include "timetestwindow.h"
#include "ui_TimeTestWindow.h"


TimeTestWindow::TimeTestWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::TimeTestWindow) {
    ui->setupUi(this);
}

TimeTestWindow::~TimeTestWindow() {
    delete ui;
}
