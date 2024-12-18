//
// Created by William Scheirey on 12/17/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LogWindow.h" resolved

#include "logwindow.h"
#include "ui_LogWindow.h"
#include "Backend/Backend.h"
#include <QFileDialog>
#include <QDesktopServices>

LogWindow::LogWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    connect(ui->OpenLogsDefault, &QPushButton::released, this, []()
    {
        if(Backend::getInstance().groundStationModem)
        {
            Backend::getInstance().groundStationModem->dataLogger->openLogsDefault();
        }
    });

    connect(ui->RevealLogs, &QPushButton::released, this, []()
    {
        if(Backend::getInstance().groundStationModem)
        {
            Backend::getInstance().groundStationModem->dataLogger->showFolder();
        }
    });

    connect(ui->ShowCSV, &QPushButton::released, this, [this]()
    {
        QString dir = DataLogger::enclosingDirectory.replace("~", QDir::homePath());
        if(Backend::getInstance().groundStationModem)
        {
            dir = Backend::getInstance().groundStationModem->dataLogger->logDir.path();
        }
        QString filepath = QFileDialog::getOpenFileName(this, "Open CSV", dir, "CSV files (*.CSV)");
        if(!filepath.isEmpty())
        {
            displayCsv(filepath);
        }
    });
}

void LogWindow::displayCsv(const QString &filepath)
{
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        QStringList headers = line.split(",");
        this->ui->CsvLogViewer->setColumnCount(headers.size());
        this->ui->CsvLogViewer->setHorizontalHeaderLabels(headers);

        int row = 0;
        while (!in.atEnd()) {
            line = in.readLine();
            QStringList fields = line.split(",");
            this->ui->CsvLogViewer->insertRow(row);
            for (int col = 0; col < fields.size(); col++) {
                this->ui->CsvLogViewer->setItem(row, col, new QTableWidgetItem(fields[col]));
            }
            row++;
        }
        file.close();
    }
}

LogWindow::~LogWindow()
{
    delete ui;
}
