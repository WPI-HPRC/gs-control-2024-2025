//
// Created by William Scheirey on 9/28/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    altitudeLabel = this->findChild<QLabel *>("Altitude");

    testButton = this->findChild<QPushButton *>("TestButton");
    if(!testButton)
    {
        qDebug() << "BAD";
    }

    connect(testButton, &QPushButton::pressed, this, &MainWindow::buttonPressed);

    altitudeLabel->setText("100000km");
}

void MainWindow::buttonPressed()
{
    qDebug() << "TEST";
    altitudeLabel->setText("TEST");
}

MainWindow::~MainWindow()
{
    delete ui;
}
