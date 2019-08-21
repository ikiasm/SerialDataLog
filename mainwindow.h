#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configserie.h"

#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void connected();
    void disconnected();

private:
    void initActionsConnections(void);
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;

    QLabel *status;
    QSerialPort *serial;
    configSerie *config;
};

#endif // MAINWINDOW_H
