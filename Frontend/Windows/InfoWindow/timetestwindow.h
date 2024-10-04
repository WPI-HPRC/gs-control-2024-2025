//
// Created by Nicolas DeBruin on 10/3/2024.
//

#ifndef TIMETESTWINDOW_H
#define TIMETESTWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class TimeTestWindow; }
QT_END_NAMESPACE

class TimeTestWindow : public QMainWindow {
Q_OBJECT

public:
    explicit TimeTestWindow(QWidget *parent = nullptr);
    ~TimeTestWindow() override;

private:
    Ui::TimeTestWindow *ui;
};


#endif //TIMETESTWINDOW_H
