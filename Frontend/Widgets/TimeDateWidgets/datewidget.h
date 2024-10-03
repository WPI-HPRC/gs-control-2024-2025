//
// Created by Nicolas DeBruin on 10/1/2024.
//

#ifndef GS_FRONTEND_2024_2025_DATEWIDGET_H
#define GS_FRONTEND_2024_2025_DATEWIDGET_H

#include <QWidget>
#include <QLabel>

#include <Utility/Utility.h>


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

    QLabel *dateText;

    //std::time_t currentLocalEpoch = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    // std::tm* currentDateTime = std::localtime(&currentLocalEpoch);

public slots:
    void newDate(int year, int month, int day);
};

#endif //DATEWIDGET_H
