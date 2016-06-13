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
    /**
     * @brief send 函数名 发送广播
     * @param string 发送的内容
     */
    void send(QString string);

    QString getNetWorkContent();
    /**
     * @brief send 函数名 向指定IP发送消息
     * @param content 发送的内容
     * @param ip 向这个地址发送
     */
    void send(QString content, QString ip);
    QString getLocalIP();
    QString getLocalHostName();
public slots:
    void recv();
    void print();
signals:
    void readSecretMessage();
    void readBroadCastMessage();
    void readGroupMessage();
};

#endif // UDP_H
