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
    void send(QString string);

    QString getNetWorkContent();
    void send(QString content, QString ip);
    QString getLocalIP();
    QString getLocalHostName();
public slots:
    void recv();
    void print();
signals:
    void readFinished();
};

#endif // UDP_H
