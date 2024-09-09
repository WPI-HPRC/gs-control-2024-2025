//
// Created by William Scheirey on 8/24/24.
//

#ifndef GS_BACKEND_2024_2025_MAINWINDOW_H
#define GS_BACKEND_2024_2025_MAINWINDOW_H

#include <QMainWindow>

#include <string>

#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>

#include "Backend/Backend.h"

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

//public slots:
//    void serialPortChosen(QListWidgetItem *, QListWidgetItem *);

private:
    void getChildren();

    Ui::MainWindow *ui;

    QPushButton *radioModuleButton;
    QTableWidget *serialPortList;

    QLineEdit *linkTest_destinationAddress;
    QPushButton *linkTest_button;
    QSpinBox *linkTest_payloadSize;
    QSpinBox *linkTest_iterations;
    QSpinBox *linkTest_repeat;

    QLabel *linkTestResults_NoiseFloor;
    QLabel *linkTestResults_MaxRssi;
    QLabel *linkTestResults_MinRssi;
    QLabel *linkTestResults_AvgRssi;
    QLabel *linkTestResults_Success;
    QLabel *linkTestResults_Retries;
    QLabel *linkTestResults_RR;

public slots:
    void linkTestDataAvailable(LinkTestResults results, int iterationsLeft);
};


#endif //GS_BACKEND_2024_2025_MAINWINDOW_H
