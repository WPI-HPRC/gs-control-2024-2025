
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

    MainWindow mainWindow;
    mainWindow.showNormal();
    mainWindow.setWindowTitle("Main Window");

    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
