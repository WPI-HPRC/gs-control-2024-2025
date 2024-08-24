
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Backend &backend = Backend::getInstance();

    MainWindow mainWindow;
    mainWindow.showNormal();
    mainWindow.update();
    mainWindow.setWindowTitle("Ground Station Control");

    backend.start();


    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
