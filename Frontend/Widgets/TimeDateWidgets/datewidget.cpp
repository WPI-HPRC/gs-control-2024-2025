//
// Created by Nicolas DeBruin on 10/1/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_DateWidget.h" resolved

#include "datewidget.h"
#include "ui_DateWidget.h"



DateWidget::DateWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DateWidget) {
    ui->setupUi(this);

    // get our reference to the text we'll modify
    dateText = this->findChild<QLabel*>("Date");

    // connect our slot to the backend
    connect(&Backend::getInstance(), &Backend::newGroundDateTime, this, &DateWidget::newDate);
}

// slot code, this updates the text when the date changes
void DateWidget::newDate(std::tm* currentDate)
{
    dateText->setText(QString::asprintf("%04d-%02d-%02d",currentDate->tm_year+1900, currentDate->tm_mon+1, currentDate->tm_mday));
    dateText->setEnabled(true);
}

DateWidget::~DateWidget() {
    delete ui;
}
