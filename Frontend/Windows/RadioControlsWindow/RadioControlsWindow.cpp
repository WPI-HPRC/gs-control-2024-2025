//
// Created by William Scheirey on 8/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "RadioControlsWindow.h"
#include <QLabel>
#include "ui_RadioControlsWindow.h"

#define FindChild(name) name = findChild<decltype(name)>(QStringLiteral(#name).replace(0, 1, QString(#name)[0].toUpper()))

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

void RadioControlsWindow::getChildren()
{
    FindChild(refreshSerialPortsButton);
    FindChild(serialPortListObj);

    FindChild(linkTest_DestinationAddress);

    FindChild(linkTest_PayloadSize);
    FindChild(linkTest_Iterations);
    FindChild(linkTest_Button);
    FindChild(linkTest_Repeat);
    FindChild(linkTest_Loop);

    FindChild(linkTestResults_NoiseFloor);
    FindChild(linkTestResults_MaxRssi);
    FindChild(linkTestResults_MinRssi);
    FindChild(linkTestResults_AvgRssi);
    FindChild(linkTestResults_Success);
    FindChild(linkTestResults_Retries);
    FindChild(linkTestResults_RR);

    FindChild(linkTestResults_TotalPackets);
    FindChild(linkTestResults_PercentSuccess);

    FindChild(throughputTest_DestinationAddress);
    FindChild(throughputTest_PayloadSize);
    FindChild(throughputTest_PacketRate);
    FindChild(throughputTest_Duration);
    FindChild(throughputTest_TransmitOptions);
    FindChild(throughputTest_Button);

    FindChild(throughputTestResults_NumSuccess);
    FindChild(throughputTestResults_PercentSuccess);
    FindChild(throughputTestResults_Throughput);
}

void RadioControlsWindow::linkTestFailed()
{
    /*
    this->linkTest_Button->setEnabled(true);
    this->linkTest_Button->setText("Run Link Test");
    */
    lastLinkTestFailed = true;
    this->linkTestButtonPressed();
    this->linkTest_Button->setEnabled(true);
    this->linkTest_Button->setText("STOP");
}

void RadioControlsWindow::linkTestDataAvailable(LinkTestResults results, int iterationsLeft)
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

    this->linkTest_Button->setEnabled(true);
    if(iterationsLeft == 0)
    {
        this->linkTest_Button->setText("Run Link Test");
    }
    else if(iterationsLeft > 0)
    {
        linkTest_Button->setText(QString::asprintf("STOP - %d left", iterationsLeft));
    }
    else
    {
        linkTest_Button->setText("STOP");
    }

    std::cout << "New link test data" << std::endl;
}

void RadioControlsWindow::linkTestButtonPressed()
{
    loopLinkTest = this->linkTest_Loop->isChecked();
    if(!lastLinkTestFailed)
    {
        if (this->linkTest_Button->text().startsWith("STOP"))
        {
            std::cout << "Stopping link test" << std::endl;
            Backend::getInstance().cancelLinkTest();
            this->linkTest_Button->setText("Run Link Test");
            loopLinkTest = false;
            return;
        }
    }

    int payloadSize = this->linkTest_PayloadSize->value();
    int iterations = this->linkTest_Iterations->value();
    int repeat = this->linkTest_Repeat->value();
    QByteArray bytes = hexToBytes(linkTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());

    linkTest_Button->setText("Running link test...");
    linkTest_Button->setEnabled(false);
    std::cout << "Last link test failed ? " << (lastLinkTestFailed ? "YES" : "NO") << std::endl;
    Backend::getInstance().runLinkTest(address, payloadSize, iterations, lastLinkTestFailed ? 0 : repeat, loopLinkTest);
    if(lastLinkTestFailed)
    {
        lastLinkTestFailed = false;
    }
}

void RadioControlsWindow::throughputTestButtonPressed()
{

    QString originatingModulePortName = serialPortListObj->getCurrentlySelectedPortName();

    if(originatingModulePortName == "")
        return;

    QByteArray bytes = hexToBytes(throughputTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());
    uint8_t payloadSize = throughputTest_PayloadSize->value();
    uint packetRate = throughputTest_PacketRate->value();
    uint duration = throughputTest_Duration->value();
    uint8_t transmitOptions = throughputTest_TransmitOptions->value();

    Backend::getInstance().runThroughputTest(originatingModulePortName, address, payloadSize, packetRate, duration, transmitOptions);
}

void RadioControlsWindow::throughputTestDataAvailable(float percentSuccess, uint numSuccess, float throughput)
{
    throughputTestResults_PercentSuccess->setEnabled(true);
    throughputTestResults_PercentSuccess->setText(QString::asprintf("%0.1f", percentSuccess));

    throughputTestResults_NumSuccess->setEnabled(true);
    throughputTestResults_NumSuccess->setText(QString::asprintf("%d", numSuccess));

    throughputTestResults_Throughput->setEnabled(true);
    throughputTestResults_Throughput->setText(QString::asprintf("%f kbps", throughput));
}

RadioControlsWindow::RadioControlsWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RadioControlsWindow)
{
    ui->setupUi(this);
    getChildren();

    linkTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    linkTestResults_NoiseFloor->setEnabled(false);
    linkTestResults_MaxRssi->setEnabled(false);
    linkTestResults_MinRssi->setEnabled(false);
    linkTestResults_AvgRssi->setEnabled(false);
    linkTestResults_Success->setEnabled(false);
    linkTestResults_Retries->setEnabled(false);
    linkTestResults_RR->setEnabled(false);

    linkTestResults_TotalPackets->setEnabled(false);
    linkTestResults_PercentSuccess->setEnabled(false);

    throughputTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    throughputTestResults_NumSuccess->setEnabled(false);
    throughputTestResults_PercentSuccess->setEnabled(false);
    throughputTestResults_Throughput->setEnabled(false);

    connect(linkTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::linkTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::linkTestDataAvailable, this, &RadioControlsWindow::linkTestDataAvailable);

    connect(&Backend::getInstance(), &Backend::linkTestFailedSignal, this, &RadioControlsWindow::linkTestFailed);

    connect(throughputTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::throughputTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::throughputTestDataAvailable, this, &RadioControlsWindow::throughputTestDataAvailable);

    connect(refreshSerialPortsButton, &QPushButton::pressed, []() {
        Backend::getInstance().getPorts();
    });


//    connect(serialPortListObj, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(serialPortChosen(QListWidgetItem*, QListWidgetItem*)));
}

/*
void RadioControlsWindow::serialPortChosen(QListWidgetItem *item, QListWidgetItem *_)
{
    radioModuleButton->setText("Connect");
    radioModuleButton->setEnabled(true);
}
 */

RadioControlsWindow::~RadioControlsWindow()
{
    delete ui;
}
