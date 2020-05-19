#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <rs232syncsender.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer= new QTimer;
    timer->setInterval(2000);
    timer->start();
    syncSender = new RS232SyncSender(this);
    connect(syncSender,&RS232SyncSender::dataReady,this,&MainWindow::getData);
    connect(timer,&QTimer::timeout,this,&MainWindow::sendTimedCommand);


}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::on_buttons2_clicked()
{
    if (requestNumber%2==0)
        syncSender->writeRequest("#DATE?\n");
    else syncSender->writeRequest("#TIME\n");
}



void MainWindow::on_pushButton_clicked()
{
    ui->lineEditSyncOutput->setText("");
}
