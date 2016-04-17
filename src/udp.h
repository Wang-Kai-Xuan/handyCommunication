#ifndef UDP_H
#define UDP_H

#include "header.h"
class Udp : public QUdpSocket
{
    Q_OBJECT
public:
    QUdpSocket * udpSocket;
    QByteArray recvData;
    QByteArray array;
    Udp();
    void configNetWork();
    void sendBroadCast(QString string);

    QString getNetWorkContent();
public slots:
    void recv();
    void print();
signals:
    void readFinished();
};

#endif // UDP_H
