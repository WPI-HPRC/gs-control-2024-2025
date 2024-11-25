
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"
#include "Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.h"
#include "Frontend/Windows/TelemetryWindow/raw_telemetry.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"

struct TestStruct
{
    uint64_t address;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Backend &backend = Backend::getInstance();

//    RadioControlsWindow radioControlsWindow;
//    radioControlsWindow.showNormal();
//    radioControlsWindow.update();
//    radioControlsWindow.setWindowTitle("Radio Controls");

    
    RadioPerformanceWindow radioPerformanceWindow;
    radioPerformanceWindow.showNormal();
    radioPerformanceWindow.update();
    radioPerformanceWindow.setWindowTitle("Live Radio Performance Stats");

//    Raw_Telemetry raw_telemetryWindow;
//    raw_telemetryWindow.showNormal();
//    raw_telemetryWindow.update();
//    raw_telemetryWindow.setWindowTitle("Raw Telemetry");

    MainWindow mainWindow;
    mainWindow.showNormal();
    mainWindow.setWindowTitle("Main Window");

    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
