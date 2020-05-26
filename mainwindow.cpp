#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <rs232syncsender.h>
#include <threadsyncserial.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer= new QTimer;
    timer->setInterval(100);

    timer2 = new QTimer();
    timer2->setInterval(100);
    syncSender = new RS232SyncSender(this);
    threadSerial = new ThreadSyncSerial(this);
    connect(syncSender,&RS232SyncSender::dataReady,this,&MainWindow::getData);
    connect(timer,&QTimer::timeout,this,&MainWindow::sendTimedCommand);
    connect(timer2,&QTimer::timeout, this, &MainWindow::sendThreadedCommand);

        connect(threadSerial, &ThreadSyncSerial::dataReady, this,&MainWindow::showRequest);
        connect(threadSerial, &ThreadSyncSerial::error, this, &MainWindow::processError);
        connect(threadSerial, &ThreadSyncSerial::timeout, this, &MainWindow::processTimeout);

}

MainWindow::~MainWindow()
{    delete ui;
}

void MainWindow::getData(QByteArray data)
{
    QString output = QString::fromStdString(data.toStdString());

    ui->lineEditSyncOutput->setText(output);
    qDebug()<<"Otrzymany text"<< output << requestNumber;

    requestNumber ++;
}



void MainWindow::sendTimedCommand()
{
    if (requestNumber%2==0)
    {

        syncSender->writeRequest("#DATE?\n");

    }
    else
    {

        syncSender->writeRequest("#TIME?\n");
    }
}


void MainWindow:: sendThreadedCommand()
{
    if (requestNumber%2==0)
    {

        threadSerial->startSlave("ttyS2",
                               "#DATE?\n");

    }
    else
    {
        threadSerial->startSlave("ttyS2",
                               "#TIME?\n");
    }
}




void MainWindow::on_buttons2_clicked()
{
    if (requestNumber%2==0)
    {

        syncSender->writeRequest("#DATE?\n");

    }
    else
    {

        syncSender->writeRequest("#TIME?\n");
    }
}



void MainWindow::on_pushButton_clicked()
{
    ui->lineEditSyncOutput->setText("");
}

void MainWindow::on_buttonStartTimer_clicked()
{
    timer->start();
}

void MainWindow::on_buttonStartTimer2_clicked()
{
    timer2->start();
}


void MainWindow::showRequest( QByteArray &s)
{
    qDebug()<<QString("\n\r-response: %3")
                            .arg(QString::fromStdString(s.toStdString()));
}

void MainWindow::processError( QString s)
{

    qDebug()<<QString("Status: Not running, %1.").arg(s);
    qDebug()<<"No traffic.";
}

void MainWindow::processTimeout( QString s)
{
    qDebug()<<QString("Status: Running, %1.").arg(s);
    qDebug()<<"No traffic.";
}
