//
// Created by William Scheirey on 8/24/24.
//

#ifndef GS_BACKEND_2024_2025_MAINWINDOW_H
#define GS_BACKEND_2024_2025_MAINWINDOW_H

#include <QMainWindow>


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


#endif //GS_BACKEND_2024_2025_MAINWINDOW_H
