
#include <QApplication>
#include "Backend/Backend.h"
#include "Frontend/Windows/MainWindow/mainwindow.h"
#include "generated/telemetry/RocketTelemetryPacket.pb.h"
#include "Utility/TelemetryHandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    MainWindow mainWindow;
    mainWindow.showMaximized();
    mainWindow.setWindowTitle("Main Window");

    Backend &backend = Backend::getInstance();
    backend.start();

    int code = QApplication::exec();

    backend.flushFiles();
    return code;
}
