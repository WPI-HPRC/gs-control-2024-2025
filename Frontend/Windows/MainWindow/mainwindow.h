//
// Created by William Scheirey on 9/28/24.
//

#ifndef GS_CONTROL_MAINWINDOW_H
#define GS_CONTROL_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};


#endif //GS_CONTROL_MAINWINDOW_H
