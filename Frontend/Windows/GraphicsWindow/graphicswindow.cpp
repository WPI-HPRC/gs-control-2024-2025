//
// Created by William Scheirey on 11/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GraphicsWindow.h" resolved

#include "graphicswindow.h"
#include "ui_GraphicsWindow.h"


GraphicsWindow::GraphicsWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
}

GraphicsWindow::~GraphicsWindow()
{
    delete ui;
}
