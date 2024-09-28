
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/RadioControlsWindow/RadioControlsWindow.h"

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


    TestStruct test;
    test.address = 0x0013A200423F474C;

    backend.start();



    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
