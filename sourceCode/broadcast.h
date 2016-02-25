#ifndef BROADCAST_H
#define BROADCAST_H
#include "chatbase.h"

class BroadCast : public ChatBase
{
    Q_OBJECT
public:
    BroadCast(QString &value);
    QByteArray recvData;
    QByteArray sendData;
    COMMAND command_send;
    QString content_send;
    COMMAND command_recv;
    QString content_recv;
    QString userId;
    QStringList strList;
    void clearRecvBuf();
    void clearSendBuf();
    void handleMessage();
    void handleBroadcast();

public slots:
    void onReadMessage(void);
    void onSendMessage(void);

};

#endif // BROADCAST_H
