#ifndef CONFIGSERIE_H
#define CONFIGSERIE_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QSerialPort>

QT_USE_NAMESPACE

namespace Ui {
class configSerie;
}

class configSerie : public QDialog
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    explicit configSerie(QWidget *parent = 0);
    ~configSerie();

    Settings settings() const;

    bool portOpened;
    bool portClosed;
    QString portName;
    QString baudRate;
    QString dataBits;
    QString parity;
    QString stopBits;
    QString flowControl;

signals:

    void serialPortOpened(int);
    void serialPortClosed(int);


private slots:

    void showPortInfo(int idx);
    void apply();
    void configPort();
    void openSerialPort();
    void closeSerialPort();

private:

    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::configSerie *ui;
    Settings currentSettings;
    QSerialPort *serial;


};

#endif // CONFIGSERIE_H
