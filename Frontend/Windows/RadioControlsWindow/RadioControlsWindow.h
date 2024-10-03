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

    QCheckBox *throughputTest_RangeScanning;
    QSpinBox *throughputTest_MinPayloadSize;
    QSpinBox *throughputTest_MaxPayloadSize;
    QSpinBox *throughputTest_PayloadSizeStep;
    QSpinBox *throughputTest_MinPacketRate;
    QSpinBox *throughputTest_MaxPacketRate;
    QSpinBox *throughputTest_PacketRateStep;

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
    void rangeScanningBoxClicked(bool checked);

    void throughputTestDataAvailable(float percentSuccess, uint numSuccess, float throughput);
    void throughputTestButtonPressed();

private:
    void disableRangeScanningOptions();
    void enableRangeScanningOptions();
};


#endif //GS_BACKEND_2024_2025_MAINWINDOW_H
