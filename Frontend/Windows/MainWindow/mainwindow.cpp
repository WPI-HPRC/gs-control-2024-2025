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

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serialPortList = this->findChild<QTableWidget *>("serialPortList");
    linkTest_destinationAddress = this->findChild<QLineEdit *>("LinkTest_DestinationAddress");
    linkTest_destinationAddress->setInputMask("HH HH HH HH HH HH HH HH");

    linkTest_payloadSize = this->findChild<QSpinBox *>("LinkTest_PayloadSize");
    linkTest_iterations = this->findChild<QSpinBox *>("LinkTest_Iterations");
    linkTest_button = this->findChild<QPushButton *>("LinkTest_Button");

    connect(linkTest_button, &QPushButton::pressed, [this](){
        int payloadSize = this->linkTest_payloadSize->value();
        int iterations = this->linkTest_iterations->value();
        QByteArray bytes = hexToBytes(linkTest_destinationAddress->text());
        uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());

        Backend::getInstance().runLinkTest(address, payloadSize, iterations);
    });

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
