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

    Ui::RadioControlsWindow *ui;

    bool loopLinkTest = false;
    bool lastLinkTestFailed = false;

    bool throughputTestIsRunning = false;

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
