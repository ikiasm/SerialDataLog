#include "configserie.h"
#include "ui_configserie.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>
#include <QLineEdit>

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("ConfigSerie", "N/A");


configSerie::configSerie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configSerie)
{
    ui->setupUi(this);

    serial = new QSerialPort;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(apply()));

    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &configSerie::showPortInfo);

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
}

configSerie::~configSerie()
{
    delete ui;
}

configSerie::Settings configSerie::settings() const
{
    return currentSettings;
}

void configSerie::showPortInfo(int idx)
{
    if (idx == -1)
        return;
    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));

}

void configSerie::apply()
{

    updateSettings();
    hide();

}

void configSerie::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("57600"),QSerialPort::Baud57600);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void configSerie::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;


    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos)
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(),16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(),16) : blankString);
        ui->serialPortInfoListBox->addItem(list.first(),list);
    }

}



void configSerie::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();
    if(ui->baudRateBox->currentIndex() == 4)
    {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }

    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.stringFlowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

}

void configSerie::configPort()
{
    show();
}

void configSerie::openSerialPort()
{
    configSerie::Settings p = this->settings();

    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    if(serial->open(QIODevice::ReadWrite))
    {

        portOpened = 1;
        portName = p.name;
        baudRate = p.stringBaudRate;
        dataBits = p.stringDataBits;
        parity = p.stringParity;
        stopBits = p.stringStopBits;
        flowControl = p.stringFlowControl;

        emit serialPortOpened(portOpened);
   }
    else
    {

        portOpened = 0;

//        QMessageBox::critical(this, tr("Error"), serial->errorString());
//        showStatusMessage(tr("Open error"));
    }
}

void configSerie::closeSerialPort()
{
    if(serial->isOpen()) serial->close();

    portClosed = 1;

    emit serialPortClosed(portClosed);
}
