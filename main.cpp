
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"
#include "Frontend/Windows/InfoWindow/timetestwindow.h"

struct TestStruct
{
    uint64_t address;
};

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    Backend &backend = Backend::getInstance();

    // RadioControlsWindow radioControlsWindow;
    // radioControlsWindow.showNormal();
    // radioControlsWindow.update();
    // radioControlsWindow.setWindowTitle("Radio Controls");

    TimeTestWindow time_test_window;
    time_test_window.showNormal();
    time_test_window.update();
    time_test_window.setWindowTitle("Test Window");



    TestStruct test;
    test.address = 0x0013A200423F474C;

    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
