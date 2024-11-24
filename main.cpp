
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"
#include "Frontend/Windows/RadioPerformanceWindow/radioperformancewindow.h"

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

    TestStruct test;
    test.address = 0x0013A200423F474C;

    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
