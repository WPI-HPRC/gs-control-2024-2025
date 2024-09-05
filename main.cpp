
#include <QApplication>
#include "Backend/Backend.h"
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
    mainWindow.update();
    mainWindow.setWindowTitle("Ground Station Control");


    TestStruct test;
    test.address = 0x0013A200423F474C;

    backend.start();



    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
