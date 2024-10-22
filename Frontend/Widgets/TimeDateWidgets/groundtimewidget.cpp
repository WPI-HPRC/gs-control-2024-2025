//
// Created by Nicolas DeBruin on 10/3/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GroundTimeWidget.h" resolved

#include "groundtimewidget.h"
#include "ui_GroundTimeWidget.h"


GroundTimeWidget::GroundTimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::GroundTimeWidget) {
    ui->setupUi(this);

    // connect our slot to the backend
    connect(&Backend::getInstance(), &Backend::newGroundDateTime, this, &GroundTimeWidget::newTime);
}

void GroundTimeWidget::newTime(std::tm* currentTime)
{
    ui->GroundTime->setText(QString::asprintf("%02d:%02d:%02d",currentTime->tm_hour,currentTime->tm_min,currentTime->tm_sec));
    ui->GroundTime->setEnabled(true);
}

GroundTimeWidget::~GroundTimeWidget() {
    delete ui;
}
