#ifndef RS232SYNCSENDER_H
#define RS232SYNCSENDER_H

#include <QObject>

class QSerialPort;
class RS232SyncSender: public QObject
{

    Q_OBJECT

public:
    RS232SyncSender(QObject * parent);
    ~RS232SyncSender();
    void writeRequest(QByteArray data);
    void processResponse(const QByteArray &data);


private:
        QSerialPort * m_serial_port;
         QByteArray buffer;
         bool m_frame_begin = 0;


signals:
    void dataReady(QByteArray response);

public slots:
    void handleReadyRead();

};

#endif // RS232SYNCSENDER_H
