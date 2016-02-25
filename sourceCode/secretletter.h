#ifndef SECRETLETTER_H
#define SECRETLETTER_H

#include <QWidget>
#include "broadcast.h"
#include "chatbase.h"
class SecretLetter : public ChatBase
{
    Q_OBJECT
public:
    explicit SecretLetter(QString &value, QString &value1);
public:

    QByteArray recvData;
    QByteArray sendData;
    COMMAND command_send;
    QString content_send;
    COMMAND command_recv;
    QString content_recv;
    QString ownerId;
    QString objectId;
    QStringList strList;
    QSqlDatabase sysDB;
    QHostAddress object_address;
    void clearRecvBuf();
    void clearSendBuf();
    void handleMessage();
    void handleSecret();

    void initDB();

signals:

public slots:
    void onReadMessage(void);
    void onSendMessage(void);

};

#endif // SECRETLETTER_H
