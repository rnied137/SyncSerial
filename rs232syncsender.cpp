#include "rs232syncsender.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>

RS232SyncSender::RS232SyncSender(QObject * parent)
    :QObject(parent)
{
    m_serial_port = new QSerialPort(this);

    connect(m_serial_port, &QSerialPort::readyRead,
            this, &RS232SyncSender::handleReadyRead);
}



void RS232SyncSender::handleReadyRead()
{

    if (!m_serial_port->isOpen()) {

        qDebug()<<"Port not open my lord";
        return;
}
    processResponse(m_serial_port->readAll());
}


//#12312312312?!!@#'n!@#123123?#???13123

void RS232SyncSender::processResponse(const QByteArray &data)

{

    qDebug()<<"DATA " << QString::fromStdString(data.toStdString());



    if (data.startsWith("#") && (data.contains('\n')))
    {
        buffer.append(data);
        emit dataReady(buffer);
        buffer.clear();
    }
        else if (data.startsWith("#"))
        {
            buffer.append(data);
            m_frame_begin = 1;
            qDebug()<<"Frame Begin";


        }

        else if(!data.startsWith("#") && (!data.contains('\n') && (m_frame_begin == 1)))
        {
            buffer.append(data);
            qDebug()<<"middle";

        }

        else if( (data.contains('\n') )&& (m_frame_begin == 1) )
         {
            qDebug()<<"Contains 'n" <<"databegin = 1";
             m_frame_begin=0;
             buffer.append(data);
             emit dataReady(buffer);
             buffer.clear();
         }

}


RS232SyncSender::~RS232SyncSender(){
    m_serial_port->close();
}

void RS232SyncSender::writeRequest(QByteArray data)
{
    m_serial_port->setBaudRate(115200);
    m_serial_port->setPortName("ttyS2");
    m_serial_port->open(QIODevice::ReadWrite);
    if (m_serial_port->isOpen()) {qDebug()<<"Port is open";}
    m_serial_port->write(data);
}
