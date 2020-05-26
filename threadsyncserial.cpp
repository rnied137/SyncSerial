#include "threadsyncserial.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTime>
#include <QSerialPort>

ThreadSyncSerial::ThreadSyncSerial(QObject * parent)
    :QThread(parent)
{





}


ThreadSyncSerial::~ThreadSyncSerial(){
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}

void ThreadSyncSerial::startSlave(const QString &portName, const QString &response)
{
    //! [1]
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = 60;
    m_response = response;
    //! [3]
    if (!isRunning())
        start();
}


void ThreadSyncSerial::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();
    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }


    int currentWaitTimeout = m_waitTimeout;
    QString currentRespone = m_response;
    m_mutex.unlock();


    QSerialPort m_serial_port;
    while (!m_quit) {
        if (currentPortNameChanged) {
            m_serial_port.close();
            m_serial_port.setPortName(currentPortName);

            if (!m_serial_port.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(m_serial_port.error()));
                return;
            }
        }

        if (m_serial_port.waitForReadyRead(currentWaitTimeout)) {


            const QByteArray requestData = m_response.toUtf8();
            m_serial_port.write(requestData);
            if (m_serial_port.waitForBytesWritten(m_waitTimeout)) {

                if (m_serial_port.bytesAvailable() || m_serial_port.waitForReadyRead(currentWaitTimeout)) {
                    QByteArray responseData = m_serial_port.readAll();
                    while (m_serial_port.waitForReadyRead(10))
                    {
                        QByteArray newData = m_serial_port.readAll();

                        if (newData.contains('\n'))//
                        {

                            qDebug()<<"Znaleziono \n";
                            responseData.append(newData);
                            emit dataReady(responseData);
                            responseData.clear();

                        }

                        else {
                            responseData.append(newData);
                            qDebug()<<"Znaleziono część";

                        }
                    }

                } else {
                    emit timeout(tr("Wait write response timeout %1")
                                 .arg(QTime::currentTime().toString()));
                }
            } else {
                emit timeout(tr("Wait read request timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
            m_mutex.lock();
            if (currentPortName != m_portName) {
                currentPortName = m_portName;
                currentPortNameChanged = true;
            } else {
                currentPortNameChanged = false;
            }
            currentWaitTimeout = m_waitTimeout;
            currentRespone = m_response;
            m_mutex.unlock();
        }
    }
}


