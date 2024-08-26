//
// Created by William Scheirey on 8/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectToRadioModuleLabel = this->findChild<QLabel *>("ConnectToRadioModuleLabel");
    serialPortList = this->findChild<QListWidget *>("serialPortList");

    connect(serialPortList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(serialPortChosen(QListWidgetItem*, QListWidgetItem*)));

    connectToRadioModuleLabel->setText("No serial ports found");
}

void MainWindow::serialPortChosen(QListWidgetItem *item, QListWidgetItem *_)
{
    connectToRadioModuleLabel->setText(QString("Connect to ").append(item->text()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
