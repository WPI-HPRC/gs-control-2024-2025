//
// Created by William Scheirey on 12/17/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LogWindow.h" resolved

#include "logwindow.h"
#include "ui_LogWindow.h"
#include "Backend/Backend.h"

LogWindow::LogWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::LogWindow)
{
    ui->setupUi(this);

    connect(ui->OpenLogsDefault, &QPushButton::pressed, this, [this]()
    {
//        Backend::getInstance().groundStationModem->dataLogger->openLogsDefault();
        Backend::getInstance().dummyLogger->openLogsDefault();

        QFile file("/Users/will/Documents/HPRC/LinkTest.csv");
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
                for (int col = 0; col < fields.size(); ++col) {
                    this->ui->CsvLogViewer->setItem(row, col, new QTableWidgetItem(fields[col]));
                }
                row++;
            }
            file.close();
        }
    });
}

LogWindow::~LogWindow()
{
    delete ui;
}
