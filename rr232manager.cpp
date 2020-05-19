#include "rr232manager.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTime>
#include <QDebug>

RR232Manager::RR232Manager(QObject * parent
                           )
    :QThread(parent)
{
    m_try_serial = new QSerialPort(this);

    //
    //m_try_serial->setPortName("ttyUSB0");  //TUTAJ!!!!!!!!!!!!!!!!

    //

    connect(m_try_serial, &QSerialPort::readyRead, this, &RR232Manager::handleReadyRead);

}


void RR232Manager::setRS232Settings(int timeout,
                                    QString portName,
                                    int baud_rate
                                    )
{
    m_timeout = timeout;
    m_portName = portName;
    m_baudrate = baud_rate;
    m_changedName = 1;
}




void RR232Manager::handleReadyRead()
{
    QByteArray responseData  = m_try_serial->readAll();
    while (m_try_serial->waitForReadyRead(30))
        responseData+= m_try_serial->readAll();

    QString response = QString::fromUtf8(responseData);
    qDebug()<<QString("Response: %1").arg(response);
    m_try_serial->close();
}


void RR232Manager::writeRequest(QByteArray  data)
{
    m_try_serial->open(QIODevice::ReadWrite);
    m_try_serial->write(data);
}


RR232Manager::~RR232Manager()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait() ;
}

void RR232Manager::getAvailablePorts()
{
        const auto infos = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &info:infos)
        {
            m_available_ports.append(info.portName());
            qDebug() << info.portName();

        }



}

void RR232Manager::getResponse(QString request)
{

}

/**
 * @brief RR232Manager::run
 */
void RR232Manager::run()
{

    m_mutex.unlock();
    QSerialPort m_serial;
    m_serial.setBaudRate(m_baudrate);
    m_serial.setPortName(m_portName);
    qDebug() << "Thread is working";
    qDebug() << "serial port name " << m_serial.portName();
    if(!m_serial.open(QIODevice::ReadWrite))
           {
               qDebug() << "Returning - device not open";
           }

    while(!m_quit)
    {


//


//        m_serial.open(QIODevice::ReadWrite);
//        m_serial.flush();

        qDebug() << m_serial.isOpen();
        int currentWaitTimeout = m_timeout;
        QString currentRequest = m_request;
        QString response;
        qDebug() << "Current Request " << currentRequest;
        const QByteArray requestData = currentRequest.toAscii();
        m_serial.write(requestData);

        qDebug()<<"Sendind data "<<currentRequest << "from port" << m_portName << "z predkoscia"<<
          m_baudrate;
        if(m_serial.waitForBytesWritten(-1)){

            // if(m_serial.waitForReadyRead(currentWaitTimeout)) {

            QByteArray reponseData = m_serial.readAll();
            while (m_serial.waitForReadyRead(30))
                reponseData+= m_serial.readAll();

          /*  QString response = QString::fromUtf8(reponseData);
            if(!response.isEmpty())
                emit this->dataReady(response);
          */


            /*}

            //else
            {
                emit this->dataReady(" Read timeout");
                //qDebug() << "Wait read response timeout";
            }
*/

        }
        else
        {
            emit this->dataReady("Write request timeout");
            //qDebug() << "Wait write request timeout";

        }



        m_mutex.lock();
        m_cond.wait(&m_mutex);
        currentWaitTimeout = m_timeout;
        //m_serial.close();
            qDebug()<< "Jestem w wait condition";
        if(m_changedName) {
            m_serial.setBaudRate(m_baudrate);
            m_serial.setPortName(m_portName);
            m_changedName = 0;
        }
        m_mutex.unlock();
        // else {
        //emit this->dataReady(tr("Response timouet %1").arg(QTime::currentTime().toString()));

        //}

    }
}




void RR232Manager::processResponse(const QByteArray &data)
{
m_frame.append(data);

if(data.contains('\n'))
{
    emit dataReady(m_frame);
}

void RR232Manager::sendResponse(QString request)
{
    m_request = request;
    qDebug() << "Setting request to" << m_request;

    if(!isRunning())
        start();
    else
        m_cond.wakeOne();
}

