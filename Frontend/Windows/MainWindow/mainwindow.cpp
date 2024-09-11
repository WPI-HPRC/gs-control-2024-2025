//
// Created by William Scheirey on 8/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include <QLabel>
#include "ui_MainWindow.h"

uint64_t getAddressBigEndian(const uint8_t *packet, size_t *index_io)
{
    uint64_t address = 0;

    for (int i = 0; i < 8; i++)
    {
        address |= (uint64_t) packet[*index_io] << (8 * (7 - i));
        (*index_io)++;
    }

    return address;
}

uint64_t getAddressBigEndian(const uint8_t *packet)
{
    size_t _ = 0;

    return getAddressBigEndian(packet, &_);
}

QByteArray hexToBytes(const QString &hexString)
{
    QByteArray rawBytes;
    QString cleanHexString = hexString;
    cleanHexString.remove(QRegularExpression("\\s")); // Remove all spaces and newlines
    rawBytes = QByteArray::fromHex(cleanHexString.toLatin1());
    return rawBytes;
}

void MainWindow::getChildren()
{
    refreshSerialPortsButton = this->findChild<QPushButton *>("RefreshSerialPortsButton");

    serialPortList = this->findChild<QTableWidget *>("serialPortList");
    linkTest_destinationAddress = this->findChild<QLineEdit *>("LinkTest_DestinationAddress");

    linkTest_payloadSize = this->findChild<QSpinBox *>("LinkTest_PayloadSize");
    linkTest_iterations = this->findChild<QSpinBox *>("LinkTest_Iterations");
    linkTest_button = this->findChild<QPushButton *>("LinkTest_Button");
    linkTest_repeat = this->findChild<QSpinBox *>("LinkTest_Repeat");
    linkTest_loop = this->findChild<QCheckBox *>("LinkTest_Loop");

    linkTestResults_NoiseFloor = this->findChild<QLabel *>("LinkTestResult_NoiseFloor");
    linkTestResults_MaxRssi = this->findChild<QLabel *>("LinkTestResult_MaxRssi");
    linkTestResults_MinRssi = this->findChild<QLabel *>("LinkTestResult_MinRssi");
    linkTestResults_AvgRssi = this->findChild<QLabel *>("LinkTestResult_AvgRssi");
    linkTestResults_Success = this->findChild<QLabel *>("LinkTestResult_Success");
    linkTestResults_Retries = this->findChild<QLabel *>("LinkTestResult_Retries");
    linkTestResults_RR = this->findChild<QLabel *>("LinkTestResult_RR");

    linkTestResults_TotalPackets = this->findChild<QLabel *>("LinkTestResults_TotalPackets");
    linkTestResults_PercentSuccess = this->findChild<QLabel *>("LinkTestResults_PercentSuccess");
}

void MainWindow::linkTestFailed()
{
    /*
    this->linkTest_button->setEnabled(true);
    this->linkTest_button->setText("Run Link Test");
    */
    lastLinkTestFailed = true;
    this->linkTestButtonPressed();
    this->linkTest_button->setEnabled(true);
    this->linkTest_button->setText("STOP");
}

void MainWindow::linkTestDataAvailable(LinkTestResults results, int iterationsLeft)
{
    linkTestResults_NoiseFloor->setText(QString::asprintf("-%d dBm", results.noiseFloor));
    linkTestResults_NoiseFloor->setEnabled(true);

    linkTestResults_MaxRssi->setEnabled(true);
    linkTestResults_MaxRssi->setText(QString::asprintf("-%d dBm", results.maxRssi));

    linkTestResults_MinRssi->setEnabled(true);
    linkTestResults_MinRssi->setText(QString::asprintf("-%d dBm", results.minRssi));

    linkTestResults_AvgRssi->setEnabled(true);
    linkTestResults_AvgRssi->setText(QString::asprintf("-%d dBm", results.avgRssi));

    linkTestResults_Success->setEnabled(true);
    linkTestResults_Success->setText(QString::asprintf("%d", results.success));

    linkTestResults_Retries->setEnabled(true);
    linkTestResults_Retries->setText(QString::asprintf("%d", results.retries));

    linkTestResults_RR->setEnabled(true);
    linkTestResults_RR->setText(QString::asprintf("%d", results.RR));

    linkTestResults_TotalPackets->setEnabled(true);
    linkTestResults_TotalPackets->setText(QString::asprintf("%d", results.iterations + results.retries));

    linkTestResults_PercentSuccess->setEnabled(true);
    linkTestResults_PercentSuccess->setText(QString::asprintf("%d", (int)((float)results.success / ((float)results.iterations + (float)results.retries) * 100)));

    this->linkTest_button->setEnabled(true);
    if(iterationsLeft == 0)
    {
        this->linkTest_button->setText("Run Link Test");
    }
    else if(iterationsLeft > 0)
    {
        linkTest_button->setText(QString::asprintf("STOP - %d left", iterationsLeft));
    }
    else
    {
        linkTest_button->setText("STOP");
    }

    std::cout << "New link test data" << std::endl;
}

void MainWindow::linkTestButtonPressed()
{
    loopLinkTest = this->linkTest_loop->isChecked();
    if(!lastLinkTestFailed)
    {
        if (this->linkTest_button->text().startsWith("STOP"))
        {
            std::cout << "Stopping link test" << std::endl;
            Backend::getInstance().cancelLinkTest();
            this->linkTest_button->setText("Run Link Test");
            loopLinkTest = false;
            return;
        }
    }

    int payloadSize = this->linkTest_payloadSize->value();
    int iterations = this->linkTest_iterations->value();
    int repeat = this->linkTest_repeat->value();
    QByteArray bytes = hexToBytes(linkTest_destinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());

    linkTest_button->setText("Running link test...");
    linkTest_button->setEnabled(false);
    std::cout << "Last link test failed ? " << (lastLinkTestFailed ? "YES" : "NO") << std::endl;
    Backend::getInstance().runLinkTest(address, payloadSize, iterations, lastLinkTestFailed ? 0 : repeat, loopLinkTest);
    if(lastLinkTestFailed)
    {
        lastLinkTestFailed = false;
    }
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    getChildren();

    linkTest_destinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    linkTestResults_NoiseFloor->setEnabled(false);
    linkTestResults_MaxRssi->setEnabled(false);
    linkTestResults_MinRssi->setEnabled(false);
    linkTestResults_AvgRssi->setEnabled(false);
    linkTestResults_Success->setEnabled(false);
    linkTestResults_Retries->setEnabled(false);
    linkTestResults_RR->setEnabled(false);

    linkTestResults_TotalPackets->setEnabled(false);
    linkTestResults_PercentSuccess->setEnabled(false);

    connect(linkTest_button, &QPushButton::pressed, this, &MainWindow::linkTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::linkTestFailedSignal, this, &MainWindow::linkTestFailed);

    connect(refreshSerialPortsButton, &QPushButton::pressed, []() {
        Backend::getInstance().getPorts();
    });

    connect(&Backend::getInstance(), &Backend::linkTestDataAvailable, this, &MainWindow::linkTestDataAvailable);

//    connect(serialPortList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(serialPortChosen(QListWidgetItem*, QListWidgetItem*)));
}

/*
void MainWindow::serialPortChosen(QListWidgetItem *item, QListWidgetItem *_)
{
    radioModuleButton->setText("Connect");
    radioModuleButton->setEnabled(true);
}
 */

MainWindow::~MainWindow()
{
    delete ui;
}
