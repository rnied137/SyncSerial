#ifndef RS232SYNCSENDER_H
#define RS232SYNCSENDER_H

#include <QObject>


#include <QSerialPort>


class RS232SyncSender: public QObject
{

    Q_OBJECT

public:
    RS232SyncSender(QObject * parent);
    ~RS232SyncSender();
    void writeRequest( const QByteArray & data);
    void processResponse(const QByteArray &data);


private:
        QSerialPort * m_serial_port;
         QByteArray buffer;
         bool m_frame_begin = 0;
         int proc_number = 0;
         QByteArray m_data;
        bool start = 0;
        bool end = 0;
        bool middle = 0;



signals:
    void dataReady(QByteArray response);

public slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleTimeout();



};

#endif // RS232SYNCSENDER_H
