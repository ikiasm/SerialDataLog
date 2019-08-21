
#include "mainwindow.h"
#include "ui_mainwindow.h"

//Includes necesarios
#include "configserie.h"
#include "QLabel"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //creacion de objetos que voy a utilizar
    //estos objetos deben declararse antes en el .h
    status = new QLabel;
    config = new configSerie;

    //agrego un widget al statusBar
    ui->statusBar->addWidget(status);

    initActionsConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initActionsConnections()
{
    //connect(objeto EMISOR, SIGNAL(evento), objeto RECEPTOR, SLOT(evento))
    //Config Puerto
    connect(ui->actionConfig, SIGNAL(triggered(bool)), config, SLOT(configPort()));

    //Estos connect estan hechos por separado, pero podria implementarse vinculando los emit
    //preparados en los metodos de configserie.cpp [openSerialPort() y closeSerialPort()]

    //Abre el puerto serie
    connect(ui->actionConnect, SIGNAL(triggered(bool)), config, SLOT(openSerialPort()));
    connect(ui->actionConnect, SIGNAL(triggered(bool)), this, SLOT(connected()));

    //cierra el puerto serie
    connect(ui->actionDisconnect, SIGNAL(triggered(bool)), config, SLOT(closeSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered(bool)), this, SLOT(disconnected()));

}

//SLOT
void MainWindow::connected()
{
    if(config->portOpened)
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionDownload_to_device->setEnabled(true);
        ui->actionUpload_from_device->setEnabled(true);

        showStatusMessage(tr("Connected to %1: %2, %3, %4, %5, %6")
                          .arg(config->portName)
                          .arg(config->baudRate)
                          .arg(config->dataBits)
                          .arg(config->parity)
                          .arg(config->stopBits)
                          .arg(config->flowControl)
                          );
   }

    else
    {
        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::disconnected()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionDownload_to_device->setEnabled(false);
    ui->actionUpload_from_device->setEnabled(false);

    showStatusMessage(tr("closed port"));
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}
