#include "udp.h"

Udp::Udp()
{
    configNetWork();
}

void Udp::configNetWork()
{
    udpSocket = new QUdpSocket(this);
    if(!udpSocket->bind(iport,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)){
        qDebug()<<"绑定端口失败";
    }
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(recv()));
}

void Udp::sendBroadCast(QString string)
{
    array = string.toUtf8();
    udpSocket->writeDatagram(array,QHostAddress::Broadcast,iport);
}
void Udp::recv(void)
{
    while(udpSocket->hasPendingDatagrams()){
        recvData.resize(udpSocket->pendingDatagramSize());
        qint64 dataSize = udpSocket->readDatagram(recvData.data(),recvData.size());
        if(dataSize == -1){
            qDebug()<<udpSocket->errorString();
        }
    }
    emit readFinished();
}

void Udp::print(void)
{
    qDebug()<<"recvData="<<getNetWorkContent()<<endl;
}
QString Udp::getNetWorkContent(void)
{
    static QString str;
    str = recvData.data();
    return str;
}
