#ifndef RR232MANAGER_H
#define RR232MANAGER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QSerialPort>

class QSerialPort;

class RR232Manager: public QThread
{
    Q_OBJECT

public:
    explicit RR232Manager(QObject * parent = nullptr);
    ~RR232Manager();

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

#endif // RR232MANAGER_H
