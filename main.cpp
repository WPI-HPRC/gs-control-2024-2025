
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"
#include "Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.h"
#include "Frontend/Windows/GraphicsWindow/graphicswindow.h"

#include "Frontend/Windows/TelemetryWindow/raw_telemetry.h"
struct TestStruct
{
    uint64_t address;
};

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    Backend &backend = Backend::getInstance();

    RadioControlsWindow radioControlsWindow;
    radioControlsWindow.showNormal();
    radioControlsWindow.update();
    radioControlsWindow.setWindowTitle("Radio Controls");

    
    RadioPerformanceWindow radioPerformanceWindow;
    radioPerformanceWindow.showNormal();
    radioPerformanceWindow.update();
    radioPerformanceWindow.setWindowTitle("Live Radio Performance Stats");

    Raw_Telemetry raw_telemetryWindow;
    raw_telemetryWindow.showNormal();
    raw_telemetryWindow.update();
    raw_telemetryWindow.setWindowTitle("Raw Telemetry");

    GraphicsWindow graphicsWindow;
    graphicsWindow.showNormal();
    graphicsWindow.update();
    graphicsWindow.setWindowTitle("Graphics");

    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
