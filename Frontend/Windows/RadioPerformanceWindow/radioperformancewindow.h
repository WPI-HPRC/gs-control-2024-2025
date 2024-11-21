//
// Created by Nicolas DeBruin on 10/26/2024.
//

#ifndef RADIOPERFORMANCEWINDOW_H
#define RADIOPERFORMANCEWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RadioPerformanceWindow; }
QT_END_NAMESPACE

class RadioPerformanceWindow : public QWidget {
Q_OBJECT

public:
    explicit RadioPerformanceWindow(QWidget *parent = nullptr);
    ~RadioPerformanceWindow() override;

private:
    Ui::RadioPerformanceWindow *ui;
};


#endif //RADIOPERFORMANCEWINDOW_H
