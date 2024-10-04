//
// Created by Nicolas DeBruin on 10/1/2024.
//

#ifndef GS_FRONTEND_2024_2025_DATEWIDGET_H
#define GS_FRONTEND_2024_2025_DATEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>
#include "Backend/Backend.h"


QT_BEGIN_NAMESPACE
namespace Ui { class DateWidget; }
QT_END_NAMESPACE

class DateWidget : public QWidget {
Q_OBJECT

public:
    explicit DateWidget(QWidget *parent = nullptr);
    ~DateWidget() override;

private:
    Ui::DateWidget *ui;

public slots:
    void newDate(std::tm* currentDate);
};

#endif //DATEWIDGET_H
