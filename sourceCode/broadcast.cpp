#include "broadcast.h"

BroadCast::BroadCast(QString &value)
{
    userId = value;
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    this->setWindowTitle(tr("广播中 - 简讯"));
}

void BroadCast::clearRecvBuf()
{
    recvData.clear();
    command_recv = COMMAND_NULL;
    content_recv.clear();

}
void BroadCast::clearSendBuf()
{
    sendData.clear();
    command_send = COMMAND_NULL;
    content_send.clear();
}

void BroadCast::onReadMessage()
{
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

void BroadCast::onSendMessage()
{
    if(input_message->document()->isEmpty()){
        QMessageBox msg;
        msg.warning(this,"提示","不能发送空消息");
        msg.show();
    }else{
        sendData.clear();
        clearSendBuf();
        command_send = BROADCAST;
        content_send.append(userId);
        content_send.append(SEPARATE);
        content_send.append(input_message->document()->toPlainText());
        sendData.append(command_send);
        sendData.append(content_send);
        qint64 dataSize =  udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,PORT);
        if(dataSize == -1){
            qDebug()<<"onSendMessage="<<udpSocket->error();
        }
        qDebug()<<"command_send="<<command_send;
        qDebug()<<"content_send="<<content_send;

    }
    input_message->clear();
}

void BroadCast::handleBroadcast()
{
    strList = content_recv.split(SEPARATE);
    show_message->append(QString("<编号:%1>%2").arg(strList.at(0)).arg(strList.at(1)));
}

void BroadCast::handleMessage()
{
    switch(command_recv){
    case BROADCAST:
        handleBroadcast();
        break;
    default:
        break;
    }
}
