//
// Created by William Scheirey on 8/24/24.
//

#include "RadioControlsWindow.h"
#include <QLabel>
#include <QComboBox>
#include "ui_RadioControlsWindow.h"

void ReverseBytes( void *start, int size )
{
    char *istart = static_cast<char *>(start), *iend = istart + size;
    std::reverse(istart, iend);
}

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

void RadioControlsWindow::linkTestFailed()
{
    /*
    this->linkTest_Button->setEnabled(true);
    this->linkTest_Button->setText("Run Link Test");
    */
    lastLinkTestFailed = true;
    this->linkTestButtonPressed();
    ui->LinkTest_Button->setEnabled(true);
    ui->LinkTest_Button->setText("STOP");
}

void RadioControlsWindow::linkTestDataAvailable(LinkTestResults results, int iterationsLeft)
{
    ui->LinkTestResults_NoiseFloor->setText(QString::asprintf("-%d dBm", results.noiseFloor));
    ui->LinkTestResults_NoiseFloor->setEnabled(true);

    ui->LinkTestResults_MaxRssi->setEnabled(true);
    ui->LinkTestResults_MaxRssi->setText(QString::asprintf("-%d dBm", results.maxRssi));

    ui->LinkTestResults_MinRssi->setEnabled(true);
    ui->LinkTestResults_MinRssi->setText(QString::asprintf("-%d dBm", results.minRssi));

    ui->LinkTestResults_AvgRssi->setEnabled(true);
    ui->LinkTestResults_AvgRssi->setText(QString::asprintf("-%d dBm", results.avgRssi));

    ui->LinkTestResults_Success->setEnabled(true);
    ui->LinkTestResults_Success->setText(QString::asprintf("%d", results.success));

    ui->LinkTestResults_Retries->setEnabled(true);
    ui->LinkTestResults_Retries->setText(QString::asprintf("%d", results.retries));

    ui->LinkTestResults_RR->setEnabled(true);
    ui->LinkTestResults_RR->setText(QString::asprintf("%d", results.RR));

    ui->LinkTestResults_TotalPackets->setEnabled(true);
    ui->LinkTestResults_TotalPackets->setText(QString::asprintf("%d", results.iterations + results.retries));

    ui->LinkTestResults_PercentSuccess->setEnabled(true);
    ui->LinkTestResults_PercentSuccess->setText(QString::asprintf("%d", (int)((float)results.success / ((float)results.iterations + (float)results.retries) * 100)));

    ui->LinkTest_Button->setEnabled(true);
    if(iterationsLeft == 0)
    {
        ui->LinkTest_Button->setText("Run Link Test");
    }
    else if(iterationsLeft > 0)
    {
        ui->LinkTest_Button->setText(QString::asprintf("STOP - %d left", iterationsLeft));
    }
    else
    {
        ui->LinkTest_Button->setText("STOP");
    }

    std::cout << "New link test data" << std::endl;
}

void RadioControlsWindow::linkTestButtonPressed()
{
    loopLinkTest = ui->LinkTest_Loop->isChecked();
    if(!lastLinkTestFailed)
    {
        if (ui->LinkTest_Button->text().startsWith("STOP"))
        {
            std::cout << "Stopping link test" << std::endl;
            Backend::getInstance().cancelLinkTest();
            ui->LinkTest_Button->setText("Run Link Test");
            loopLinkTest = false;
            return;
        }
    }

    int payloadSize = ui->LinkTest_PayloadSize->value();
    int iterations = ui->LinkTest_Iterations->value();
    int repeat = ui->LinkTest_Repeat->value();
    QByteArray bytes = hexToBytes(ui->LinkTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());

    ui->LinkTest_Button->setText("Running link test...");
    ui->LinkTest_Button->setEnabled(false);
    std::cout << "Last link test failed ? " << (lastLinkTestFailed ? "YES" : "NO") << std::endl;
    Backend::getInstance().runLinkTest(address, payloadSize, iterations, lastLinkTestFailed ? 0 : repeat, loopLinkTest);
    if(lastLinkTestFailed)
    {
        lastLinkTestFailed = false;
    }
}

void RadioControlsWindow::throughputTestButtonPressed()
{
    if(throughputTestIsRunning)
    {
        throughputTestIsRunning = false;
        Backend::getInstance().cancelThroughputTest();
        ui->ThroughputTest_Button->setText("Run throughput test");
        ui->ThroughputTest_ParamsContainer->setEnabled(true);
        return;
    }

    QString originatingModulePortName = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(originatingModulePortName == "")
    {
        ui->ThroughputTest_Button->setText("Choose a connected module!");
        QTimer::singleShot(1000, [this]()
        {
            this->ui->ThroughputTest_Button->setText("Run throughput test");
        });
        return;
    }

    QByteArray bytes = hexToBytes(ui->ThroughputTest_DestinationAddress->text());
    uint64_t address = getAddressBigEndian((uint8_t *)bytes.data());
    uint duration = ui->ThroughputTest_Duration->value();
    uint8_t transmitOptions = ui->ThroughputTest_TransmitOptions->value();

    if(!ui->ThroughputTest_RangeScanning->isChecked())
    {
        uint8_t payloadSize = ui->ThroughputTest_PayloadSize->value();
        uint packetRate = ui->ThroughputTest_PacketRate->value();

        Backend::getInstance().runThroughputTest(originatingModulePortName, address, payloadSize, packetRate, duration,
                                                 transmitOptions);
    }
    else
    {
        QList<QList<int>> testParams;
        // Yuck
        for(
                int payloadSize = ui->ThroughputTest_MinPayloadSize->value();
                payloadSize <= ui->ThroughputTest_MaxPayloadSize->value() + ui->ThroughputTest_PayloadSizeStep->value();
                payloadSize += ui->ThroughputTest_PayloadSizeStep->value())
        {
            for(
                    int packetRate = ui->ThroughputTest_MinPacketRate->value();
                    packetRate <= ui->ThroughputTest_MaxPacketRate->value() + ui->ThroughputTest_PacketRateStep->value();
                    packetRate += ui->ThroughputTest_PacketRateStep->value())
            {
                testParams.append({payloadSize, packetRate});
            }
        }
        std::cout << testParams.count() << " Tests" << std::endl;
        std::cout << testParams.count() * duration << " seconds of tests. (" << testParams.count() * duration/60 << " minutes)" << std::endl;

        Backend::getInstance().runThroughputTestsWithRange(originatingModulePortName, address, testParams, duration, transmitOptions);
    }
    throughputTestIsRunning = true;
    ui->ThroughputTest_Button->setText("STOP");
    ui->ThroughputTest_ParamsContainer->setEnabled(false);
}

void RadioControlsWindow::throughputTestDataAvailable(float percentSuccess, uint numSuccess, float throughput)
{
    if(Backend::getInstance().throughputTestIndex >= 0 && throughputTestIsRunning)
    {
        ui->ThroughputTest_Button->setText(QString::asprintf("STOP - %lld left",
                                                             Backend::getInstance().throughputTests.count() - Backend::getInstance().throughputTestIndex));
    }
    else
    {
        ui->ThroughputTest_Button->setText("Run throughput test");
        throughputTestIsRunning = false;
        ui->ThroughputTest_ParamsContainer->setEnabled(true);
    }

    ui->ThroughputTestResults_PercentSuccess->setEnabled(true);
    ui->ThroughputTestResults_PercentSuccess->setText(QString::asprintf("%0.1f", percentSuccess));

    ui->ThroughputTestResults_NumSuccess->setEnabled(true);
    ui->ThroughputTestResults_NumSuccess->setText(QString::asprintf("%d", numSuccess));

    ui->ThroughputTestResults_Throughput->setEnabled(true);
    ui->ThroughputTestResults_Throughput->setText(QString::asprintf("%f kbps", throughput));
}

void RadioControlsWindow::receiveAtCommandResponse(uint16_t command, const uint8_t *response, size_t response_length_bytes)
{
    // If we were setting a parameter, there is only one byte in the response (status). Otherwise, there will always be more than one byte
    if(response_length_bytes == 0)
        return;

    QString nodeID{};

    if(command == XBee::AtCommand::InterfaceDataRate)
    {
        uint8_t value = *(uint8_t*)&response[3];
        if (value > 0x0A)
        {
            return;
        }
    }
    else if(command == XBee::AtCommand::NodeIdentifier)
    {
        for (int i = 0; i < response_length_bytes; i++)
        {
            uint8_t byte = response[i];

            if (byte == 0x00)
            {
                break;
            }

            nodeID.append((char)byte);
        }
    }

    switch (command)
    {
        case XBee::AtCommand::InterfaceDataRate:
            ui->RadioParameters_BaudRate->setCurrentIndex(response[3]);
            break;

        case XBee::AtCommand::InterfaceParity:
            ui->RadioParameters_Parity->setCurrentIndex(response[0]);
            break;

        case XBee::AtCommand::InterfaceStopBits:
            ui->RadioParameters_StopBits->setCurrentIndex(response[0]);
            break;

        case XBee::AtCommand::ApiMode:
            ui->RadioParameters_ApiMode->setCurrentIndex(response[0]);
            break;

        case XBee::AtCommand::ApiOptions:
            ui->RadioParameters_ApiOptions->setCurrentIndex(response[0]);
            break;

        case XBee::AtCommand::MessagingMode:
            ui->RadioParameters_MessagingMode->setCurrentIndex(response[0]);
            break;
        case XBee::AtCommand::NodeIdentifier:
            ui->RadioParameters_NodeIdentifier->setText(nodeID);
            break;
        case XBee::AtCommand::NetworkID:
            ui->RadioParameters_NetworkID->setValue(response[0] << 8 | response[1]);
            break;
        case XBee::AtCommand::PreambleID:
            ui->RadioParameters_PreambleID->setValue(response[0]);
            break;
        case XBee::AtCommand::ClusterID:
            ui->RadioParameters_ClusterID->setValue(response[0] << 8 | response[1]);
            break;
        case XBee::AtCommand::NodeDiscoveryBackoff:
            ui->RadioParameters_NetworkDiscoveryBackOff->setValue(response[0] << 8 | response[1]);
            break;
        case XBee::AtCommand::NodeDiscoveryOptions:
            ui->RadioParameters_NetworkDiscoveryOptions->setValue(response[0]);
            break;
        case XBee::AtCommand::RFDataRate:
            ui->RadioParameters_RfDataRate->setCurrentIndex(response[0]);
            break;
        case XBee::AtCommand::PowerLevel:
            ui->RadioParameters_TxPowerLevel->setCurrentIndex(response[0]);
            break;
        case XBee::AtCommand::TransmitOptions:
            ui->RadioParameters_TransmitOptions->setValue(response[0]);
            break;
        case XBee::AtCommand::UnicastRetries:
            ui->RadioParameters_UnicastRetries->setValue(response[0]);
            break;
        case XBee::AtCommand::MeshUnicastRetries:
            ui->RadioParameters_MeshUnicastRetries->setValue(response[0]);
            break;
        case XBee::AtCommand::NetworkHops:
            ui->RadioParameters_NetworkHops->setValue(response[0]);
            break;
        case XBee::AtCommand::BroadcastHops:
            ui->RadioParameters_BroadcastHops->setValue(response[0]);
            break;
        case XBee::AtCommand::BroadcastMultiTransmits:
            ui->RadioParameters_BroadcastMultiTransmits->setValue(response[0]);
            break;
        default:
            return;
    }
}

void RadioControlsWindow::rangeScanningBoxClicked(bool checked)
{
    if(checked)
    {
        ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(true);
    }
    else
    {
        ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(false);
    }
}

void RadioControlsWindow::baudRateSelected(const QString &baudRateString)
{
    int baudRate = baudRateString.toInt();

    QString currentPort = ui->SerialPortListObj->getCurrentlySelectedPortName();
    
    if(currentPort == "")
    {
        return;
    }

    Backend::getInstance().setBaudRate(currentPort, baudRate);
}

void RadioControlsWindow::newBytesRead(const QString& text)
{
    ui->BytesReadBrowser->append(text);
}

void RadioControlsWindow::newBytesWritten(const QString& text)
{
    ui->BytesWrittenBrowser->append(text);
}


void RadioControlsWindow::readSerialParameters()
{
    QString currentPort = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(currentPort == "")
    {
        return;
    }

    Backend::getInstance().queryParameters(currentPort, {
        AsciiToUint16('B', 'D'),
        AsciiToUint16('N', 'B'),
        AsciiToUint16('S', 'B'),
        AsciiToUint16('A', 'P'),
        AsciiToUint16('A', 'O')
    });
}

void RadioControlsWindow::writeSerialParameters()
{
    QString currentPort = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(currentPort == "")
    {
        return;
    }

    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::InterfaceDataRate, ui->RadioParameters_BaudRate->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::InterfaceParity, ui->RadioParameters_Parity->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::InterfaceStopBits, ui->RadioParameters_StopBits->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::ApiMode, ui->RadioParameters_ApiMode->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::ApiOptions, ui->RadioParameters_ApiOptions->currentIndex());
    Backend::getInstance().writeParameters(currentPort);
}

void RadioControlsWindow::readMessagingParameters()
{
    QString currentPort = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(currentPort == "")
    {
        return;
    }

    Backend::getInstance().queryParameters(currentPort, {
            AsciiToUint16('C', 'E'),
            AsciiToUint16('N', 'I'),
            AsciiToUint16('I', 'D'),
            AsciiToUint16('H', 'P'),
            AsciiToUint16('C', 'I'),
            AsciiToUint16('N', 'T'),
            AsciiToUint16('N', 'O'),
            AsciiToUint16('B', 'R'),
            AsciiToUint16('P', 'L'),
            AsciiToUint16('T', 'O'),
            AsciiToUint16('R', 'R'),
            AsciiToUint16('M', 'R'),
            AsciiToUint16('N', 'H'),
            AsciiToUint16('B', 'H'),
            AsciiToUint16('M', 'T'),

    });
}

void RadioControlsWindow::writeMessagingParameters()
{
    QString currentPort = ui->SerialPortListObj->getCurrentlySelectedPortName();

    if(currentPort == "")
    {
        return;
    }

    QByteArray nodeID = ui->RadioParameters_NodeIdentifier->text().toUtf8();

    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::MessagingMode, ui->RadioParameters_MessagingMode->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::NodeIdentifier, (uint8_t *)nodeID.data(), nodeID.length());

    uint16_t networkID = ui->RadioParameters_NetworkID->value();
    XBeeDevice::reverseBytes(&networkID, 2); // Need to go from little -> big endian
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::NetworkID, (uint8_t *)&networkID, 2);
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::PreambleID, ui->RadioParameters_PreambleID->value());

    uint16_t clusterID = ui->RadioParameters_ClusterID->value();
    XBeeDevice::reverseBytes(&clusterID, 2);
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::ClusterID, (uint8_t *)&clusterID, 2);

    uint16_t discoveryBackoff = ui->RadioParameters_NetworkDiscoveryBackOff->value();
    XBeeDevice::reverseBytes(&discoveryBackoff, 2);
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::NodeDiscoveryBackoff, (uint8_t *)&discoveryBackoff, 2);


    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::NodeDiscoveryOptions, ui->RadioParameters_NetworkDiscoveryOptions->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::RFDataRate, ui->RadioParameters_RfDataRate->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::PowerLevel, ui->RadioParameters_TxPowerLevel->currentIndex());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::TransmitOptions, ui->RadioParameters_TransmitOptions->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::UnicastRetries, ui->RadioParameters_UnicastRetries->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::MeshUnicastRetries, ui->RadioParameters_MeshUnicastRetries->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::NetworkHops, ui->RadioParameters_NetworkHops->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::BroadcastHops, ui->RadioParameters_BroadcastHops->value());
    Backend::getInstance().setParameter(currentPort, XBee::AtCommand::BroadcastMultiTransmits, ui->RadioParameters_BroadcastMultiTransmits->value());
    Backend::getInstance().writeParameters(currentPort);
}

RadioControlsWindow::RadioControlsWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::RadioControlsWindow)
{
    ui->setupUi(this);

    ui->LinkTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    ui->LinkTestResults_NoiseFloor->setEnabled(false);
    ui->LinkTestResults_MaxRssi->setEnabled(false);
    ui->LinkTestResults_MinRssi->setEnabled(false);
    ui->LinkTestResults_AvgRssi->setEnabled(false);
    ui->LinkTestResults_Success->setEnabled(false);
    ui->LinkTestResults_Retries->setEnabled(false);
    ui->LinkTestResults_RR->setEnabled(false);

    ui->LinkTestResults_TotalPackets->setEnabled(false);
    ui->LinkTestResults_PercentSuccess->setEnabled(false);

    ui->ThroughputTest_DestinationAddress->setInputMask("HH HH HH HH HH HH HH HH");
    ui->ThroughputTestResults_NumSuccess->setEnabled(false);
    ui->ThroughputTestResults_PercentSuccess->setEnabled(false);
    ui->ThroughputTestResults_Throughput->setEnabled(false);

    ui->ThroughputTest_RangeScanningParamsContainer->setEnabled(false);
    connect(ui->ThroughputTest_RangeScanning, &QCheckBox::clicked, this, &RadioControlsWindow::rangeScanningBoxClicked);

    connect(ui->LinkTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::linkTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::linkTestDataAvailable, this, &RadioControlsWindow::linkTestDataAvailable);

    connect(&Backend::getInstance(), &Backend::linkTestFailedSignal, this, &RadioControlsWindow::linkTestFailed);

    connect(ui->ThroughputTest_Button, &QPushButton::pressed, this, &RadioControlsWindow::throughputTestButtonPressed);
    connect(&Backend::getInstance(), &Backend::throughputTestDataAvailable, this, &RadioControlsWindow::throughputTestDataAvailable);

    connect(ui->RefreshSerialPortsButton, &QPushButton::pressed, []() {
        Backend::getInstance().getPorts();
    });

    connect(ui->BaudRateDropdown, &QComboBox::textActivated, this, &RadioControlsWindow::baudRateSelected);

    connect(ui->RadioParameters_Serial_ReadButton, &QPushButton::pressed, this, &RadioControlsWindow::readSerialParameters);
    connect(ui->RadioParameters_Serial_WriteButton, &QPushButton::pressed, this, &RadioControlsWindow::writeSerialParameters);

    connect(&Backend::getInstance(), &Backend::receivedAtCommandResponse, this, &RadioControlsWindow::receiveAtCommandResponse);

    connect(&Backend::getInstance(), &Backend::newBytesReadAvailable, this, &RadioControlsWindow::newBytesRead);
    connect(&Backend::getInstance(), &Backend::newBytesWrittenAvailable, this, &RadioControlsWindow::newBytesWritten);

    connect(ui->RadioParameters_Messaging_ReadButton, &QPushButton::pressed, this, &RadioControlsWindow::readMessagingParameters);
    connect(ui->RadioParameters_Messaging_WriteButton, &QPushButton::pressed, this, &RadioControlsWindow::writeMessagingParameters);

    connect(ui->ClearBytesReadButton, &QPushButton::pressed, [this]()
    {
       ui->BytesReadBrowser->clear();
    });

    connect(ui->ClearBytesWrittenButton, &QPushButton::pressed, [this]()
    {
        ui->BytesWrittenBrowser->clear();
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
