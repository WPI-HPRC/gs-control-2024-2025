//
// Created by William Scheirey on 8/24/24.
//

#ifndef GS_BACKEND_2024_2025_MAINWINDOW_H
#define GS_BACKEND_2024_2025_MAINWINDOW_H

#include <QMainWindow>

#include <string>

#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>

#include "Backend/Backend.h"
#include "Frontend/Widgets/SerialPortList.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class RadioControlsWindow;
}
QT_END_NAMESPACE

class RadioControlsWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit RadioControlsWindow(QWidget *parent = nullptr);

    ~RadioControlsWindow() override;

//public slots:
//    void serialPortChosen(QListWidgetItem *, QListWidgetItem *);

private:
    void getChildren();

    Ui::RadioControlsWindow *ui;

    SerialPortList *serialPortListObj;

    QPushButton *refreshSerialPortsButton;

    QLineEdit *linkTest_DestinationAddress;
    QPushButton *linkTest_Button;
    QSpinBox *linkTest_PayloadSize;
    QSpinBox *linkTest_Iterations;
    QSpinBox *linkTest_Repeat;
    QCheckBox *linkTest_Loop;

    QLabel *linkTestResults_NoiseFloor;
    QLabel *linkTestResults_MaxRssi;
    QLabel *linkTestResults_MinRssi;
    QLabel *linkTestResults_AvgRssi;
    QLabel *linkTestResults_Success;
    QLabel *linkTestResults_Retries;
    QLabel *linkTestResults_RR;

    QLabel *linkTestResults_TotalPackets;
    QLabel *linkTestResults_PercentSuccess;

    QLineEdit *throughputTest_DestinationAddress;
    QSpinBox *throughputTest_PayloadSize;
    QSpinBox *throughputTest_PacketRate;
    QSpinBox *throughputTest_Duration;
    QSpinBox *throughputTest_TransmitOptions;
    QPushButton *throughputTest_Button;

    QLabel *throughputTestResults_PercentSuccess;
    QLabel *throughputTestResults_NumSuccess;
    QLabel *throughputTestResults_Throughput;

    bool loopLinkTest = false;
    bool lastLinkTestFailed = false;

public slots:
    void linkTestDataAvailable(LinkTestResults results, int iterationsLeft);
    void linkTestButtonPressed();
    void linkTestFailed();

    void throughputTestDataAvailable(float percentSuccess, uint numSuccess, uint throughput);
    void throughputTestButtonPressed();
};


#endif //GS_BACKEND_2024_2025_MAINWINDOW_H
