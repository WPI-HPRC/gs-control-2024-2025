//
// Created by William Scheirey on 12/16/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LedWidget.h" resolved

#include "ledwidget.h"
#include "ui_LedWidget.h"


LedWidget::LedWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::LedWidget)
{
    ui->setupUi(this);

    connect(&timer, &QTimer::timeout, this, &LedWidget::timedOut);

    timer.setSingleShot(true);
    timer.setInterval(timeoutDuration_sec*1000);
    timer.start();
}

void LedWidget::resetTimer()
{
    timer.stop();

    ui->Indicator->setState(QLedLabel::StateOk);

    timer.start();
}

void LedWidget::updateLabel()
{
    ui->Label->setText(this->property("Label").toString());
}

void LedWidget::setLabel(const QString &label)
{
    ui->Label->setText(label);
    ui->Label->setProperty("Label", label);
}

void LedWidget::timedOut()
{
    ui->Indicator->setState(QLedLabel::StateError);
}

LedWidget::~LedWidget()
{
    delete ui;
}
