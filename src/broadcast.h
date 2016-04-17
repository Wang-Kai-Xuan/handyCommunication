#ifndef BROADCAST_H
#define BROADCAST_H

#include "header.h"
#include "udp.h"
class BroadCast : public QWidget
{
    Q_OBJECT
public:
    explicit BroadCast(Udp * socket,QString & id,QWidget *parent = 0);
public:
    QGridLayout * glay_main;
    QTextBrowser * show_message;
    QTextEdit * input_message;
    QPushButton * send_btn;
    QLabel * message_show_lab;
    QLabel * message_input_lab;
    QString send_buf;
    QString recv_buf;
    QString & user_id;
    void newUI();
    void setUI();
    void setConnect();

public:
    Udp * udpSocket;

signals:

public slots:
    void onReadMessage(void);
    void onSendMessage();
};

#endif // ChatBase_H
