#include "rs232syncsender.h"

#include <QDebug>
#include <QSerialPortInfo>


RS232SyncSender::RS232SyncSender(QObject * parent)
    :QObject(parent)
{
    m_serial_port = new QSerialPort(this);

    connect(m_serial_port, &QSerialPort::readyRead,
            this, &RS232SyncSender::handleReadyRead);

    connect(m_serial_port, &QSerialPort::errorOccurred,
              this, &RS232SyncSender::handleError);

    m_serial_port->setBaudRate(115200);
    m_serial_port->setPortName("ttyS2");
    m_serial_port->setParity(QSerialPort::NoParity);
    m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
    m_serial_port->setDataBits(QSerialPort::DataBits::Data8);
    m_serial_port->setStopBits(QSerialPort::StopBits::OneStop);
}



void RS232SyncSender::handleReadyRead()
{

    if (!m_serial_port->isOpen()) {

        qDebug()<<"Port not open my lord";
        return;
}

    qDebug()<< " New data in bytes"<< m_serial_port->bytesAvailable()
            <<"Handle ready read number" <<proc_number
           <<"Bytes to write"<<m_serial_port->bytesToWrite();

    processResponse(m_serial_port->readAll());

    proc_number++;
}


//#12312312312?!!@#'n!@#123123?#???13123

void RS232SyncSender::processResponse(const QByteArray &data)

{
//if data # begin
    //if data
    buffer.append(data);

 while ((!buffer.startsWith("#")) && (!buffer.endsWith("\n")))
 {
     qDebug()<<"DATA"<<data;
 }

 if ((buffer.startsWith("#")) && (buffer.endsWith("\n")))
     {
     qDebug()<<"BUFFER" <<buffer;
     emit dataReady(buffer);
     buffer.clear();
    }

}


RS232SyncSender::~RS232SyncSender(){
    m_serial_port->close();
}

void RS232SyncSender::writeRequest(const QByteArray &data)
{
    m_data = data;

    if (!m_serial_port->isOpen()) {
        qDebug()<<"Port is open";
        m_serial_port->open(QIODevice::ReadWrite);}

    const qint64 bytesWritten = m_serial_port->write(data);

    if(m_serial_port->bytesToWrite()) qDebug()<<"Bytes to write"<<m_serial_port->bytesToWrite();

    if (m_serial_port->bytesAvailable()) qDebug()<<"PRE Write " <<m_serial_port->readAll();

    m_serial_port->flush();
    qDebug()<<"Trying to write data" <<QString::fromStdString(m_data.toStdString())<< "to port" << m_serial_port->portName();

    if (bytesWritten == -1) {
            qDebug() << QObject::tr("Failed to write the data to port %1, error: %2")
                                .arg(m_serial_port->portName())
                                .arg(m_serial_port->errorString())
                             << endl;

        } else if (bytesWritten != data.size()) {
            qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2")
                                .arg(m_serial_port->portName())
                                .arg(m_serial_port->errorString())
                             << endl;
        }
    qDebug()<<"Write request end";
}


void RS232SyncSender::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {

        qDebug() << QObject::tr("An I/O error occurred while writing"
                                        " the data to port %1, error: %2")
                            .arg(m_serial_port->portName())
                            .arg(m_serial_port->errorString())
                         << endl;

    }
}

void RS232SyncSender::handleTimeout()
{
    qDebug() << QObject::tr("Operation timed out for port %1, error: %2")
                        .arg(m_serial_port->portName())
                        .arg(m_serial_port->errorString())
                     << endl;
}
