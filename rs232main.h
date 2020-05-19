#ifndef RR232MAIN_H
#define RR232MAIN_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QSerialPort>

class QSerialPort;

class RS232Main: public QThread
{
    Q_OBJECT

public:
    explicit RS232Main(QObject * parent = nullptr);
    ~RS232Main();

     void getAvailablePorts();
    void sendResponse(QString request);
    void setRS232Settings(int,QString, int);


private:
    QList<QString> m_available_ports;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
    QString m_portName;
    int m_timeout;
    QSerialPort  * m_try_serial;
    int m_baudrate;
    QString m_request;

    bool m_changedName = 0;
    QByteArray m_frame;


private:
    void run() override;

signals:
    void dataReady(QByteArray r);

public slots:
    void getResponse(QString r);

    void handleReadyRead();

    void writeRequest(QByteArray data);
    void processResponse(const QByteArray &data);

};

#endif // RR232MAIN_H
