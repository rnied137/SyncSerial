#ifndef THREADSYNCSERIAL_H
#define THREADSYNCSERIAL_H

#include <QObject>

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class ThreadSyncSerial: public QThread
{

    Q_OBJECT

public:
    ThreadSyncSerial(QObject * parent);
    ~ThreadSyncSerial();
    void writeRequest(QByteArray data);
    void processResponse(const QByteArray &data);


private:
         QByteArray buffer;
         bool m_frame_begin = 0;
         int proc_number = 0;
         void appendToBuffer(QByteArray & data);



signals:
    void dataReady(QByteArray & response);

protected:
    void run();

public:
    void startSlave(const QString &portName, const QString &response);




signals:
    void error(QString s);
    void timeout(QString s);


private:
    QString m_portName;
    QString m_response;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    bool m_quit = false;




};

#endif // ThreadSyncSerial_H
