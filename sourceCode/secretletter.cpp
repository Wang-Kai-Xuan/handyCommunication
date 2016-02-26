#include "secretletter.h"
#include <QSqlQuery>
#include <QSqlError>
void SecretLetter::initDB()
{
    sysDB = QSqlDatabase::addDatabase("QSQLITE","sys_db");
    sysDB.setDatabaseName("chat.db");
    if(!sysDB.open()){
        qDebug()<<"数据库打开出错"<<sysDB.lastError();
    }
}

SecretLetter::SecretLetter(QString &value,QString &value1)
{
    ownerId = value;
    objectId = value1;
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    this->setWindowTitle(tr("与%1通讯 - 简讯").arg(objectId));

    initDB();
    QSqlQuery sql(sysDB);
    if(!sql.exec(QString("select ip from user where id = %1").arg(objectId))){
        qDebug()<<"in secret="<<sql.lastError();
    }else{
        while(sql.next()){
            qDebug()<<"ip="<<sql.value(0).toString();
            object_address.setAddress(sql.value(0).toString());
        }
    }
}

void SecretLetter::clearRecvBuf()
{
    recvData.clear();
    command_recv = COMMAND_NULL;
    content_recv.clear();
}

void SecretLetter::clearSendBuf()
{
    sendData.clear();
    command_send = COMMAND_NULL;
    content_send.clear();
}

void SecretLetter::handleMessage()
{
    switch(command_recv){
    case SECRET_CHAT:
        handleSecret();
        break;
    default:
        break;
    }
}

void SecretLetter::handleSecret()
{
    strList = content_recv.split(SEPARATE);
    show_message->append(QString("<编号:%1>%2").arg(strList.at(0)).arg(strList.at(1)));
}

void SecretLetter::onReadMessage()
{
    qDebug()<<"on read";
    clearRecvBuf();
    while(udpSocket->hasPendingDatagrams()){
        recvData.resize(udpSocket->pendingDatagramSize());
        qint64 dataSize = udpSocket->readDatagram(recvData.data(),recvData.size());
        if(dataSize == -1){
            qDebug()<<udpSocket->errorString();
        }
    }
    command_recv = (COMMAND)recvData.at(0);
    content_recv = recvData.mid(1).data();
    qDebug()<<"command_recv="<<command_recv;
    qDebug()<<"content_recv="<<content_recv;
    handleMessage();
}

void SecretLetter::onSendMessage()
{
    if(input_message->document()->isEmpty()){
        QMessageBox msg;
        msg.warning(this,"提示","不能发送空消息");
        msg.show();
    }else{
        sendData.clear();
        clearSendBuf();
        command_send = SECRET_CHAT;
        content_send.append(ownerId);
        content_send.append(SEPARATE);
        content_send.append(input_message->document()->toPlainText());
        sendData.append(command_send);
        sendData.append(content_send);
        qint64 dataSize =  udpSocket->writeDatagram(sendData.data(),sendData.size(),object_address,iport);
        if(dataSize == -1){
            qDebug()<<"onSendMessage="<<udpSocket->error();
        }
        qDebug()<<"send_address="<<object_address.toString();
        qDebug()<<"command_send="<<command_send;
        qDebug()<<"content_send="<<content_send;
    }
    input_message->clear();
}

