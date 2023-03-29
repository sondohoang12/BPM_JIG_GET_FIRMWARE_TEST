#include "mainwindow.h"
#include "ui_mainwindow.h"

QString received_data;

void MainWindow::serialportlist_reload()
{
    QSerialPortInfo info;
    QList<QSerialPortInfo> portList = info.availablePorts();
    ui->cmbboxPortList->clear();
    for(int i=0; i<portList.size(); i++)
    {
        ui->cmbboxPortList->addItem(portList[i].portName());
    }
}

void MainWindow::serialport_set(QString port)
{
    serialPort->setPortName(port);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
}

void MainWindow::serialport_read()
{
    ui->ReceivedData->moveCursor(QTextCursor::Start);

    received_data = serialPort->readAll();
    ui->ReceivedData->insertPlainText(received_data);

    qDebug()<< received_data;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("BPM Tester");
    serialportlist_reload();

    serialPort = new QSerialPort(this);
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialport_read()));

        QString exePath = "C:/Windows/System32";
        //QString exePath = "D:/DATA";

    //QString exePath = "C:\Espressif";
        pro = new QProcess(parent);
        pro->setWorkingDirectory(exePath);
        pro->setReadChannel(QProcess::StandardOutput);
        connect(pro,&QProcess::readyReadStandardOutput, this, &MainWindow::readResult);
        pro->start("cmd.exe");
        if (!pro->waitForStarted())
        {
          qDebug() << "The process didnt start" << pro->error();
        }
        //pro->write("cd C:/Users/sondo/Desktop/ESP32/ethernet_test\n\r");
        //pro->write("idf.py build\n\r");
}

MainWindow::~MainWindow()
{
    delete ui;
    serialPort->close();
}

void MainWindow::on_btnReload_clicked()
{
    QSerialPortInfo info;
    QList<QSerialPortInfo> portList = info.availablePorts();
    ui->cmbboxPortList->clear();
    for(int i=0; i<portList.size(); i++)
    {
        ui->cmbboxPortList->addItem(portList[i].portName());
    }
}

void MainWindow::on_btnFlash_clicked()
{
    if (ui->lineEdit_command->text().isEmpty())
        return;
    QByteArray cmd = QVariant(ui->lineEdit_command->text()).toByteArray();
     pro->write(cmd);
     pro->write("\n\r");
     ui->lineEdit_command->clear();
}

void MainWindow::on_btnConnect_clicked()
{
    QSerialPortInfo info;
    QString port;
    SerialPortError

    port = ui->cmbboxPortList->currentText();
    serialport_set(port);
    serialPort->open(QIODevice::ReadWrite);
    qDebug()<< (serialPort->error());

    qDebug()<< port;
}

void MainWindow::readResult()
{
    while(pro->bytesAvailable())
    {
        qDebug(pro->readAll());
    }
}

void MainWindow::on_btnDisconnect_clicked()
{
    serialPort->close();
}
