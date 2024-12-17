//
// Created by William Scheirey on 9/28/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Backend/Backend.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->EnglishUnitConversion, &QCheckBox::checkStateChanged, this, [](Qt::CheckState state)
    {
        if (state == Qt::Checked)
        {
            Backend::getInstance().convertToEnglish = true;
        }
        else
        {
            Backend::getInstance().convertToEnglish = false;
        }
    });

    connect(ui->GeeConversion, &QCheckBox::checkStateChanged, this, [](Qt::CheckState state)
    {
        if (state == Qt::Checked)
        {
            Backend::getInstance().convertFromGees = true;
        }
        else
        {
            Backend::getInstance().convertFromGees = false;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
