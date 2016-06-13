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

void Udp::send(QString string)
{
    array = string.toUtf8();
    udpSocket->writeDatagram(array,QHostAddress::Broadcast,iport);
}

void Udp::send(QString content,QString ip)
{
    array = content.toUtf8();
    QHostAddress addr;
    addr.setAddress(ip);
    udpSocket->writeDatagram(array,array.size(),addr,iport);
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

    qDebug()<<"in recv="<<recvData;
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
QString Udp::getLocalHostName()
{
    static QString localHostName = QHostInfo::localHostName();
    return localHostName;

}
QString Udp::getLocalIP()
{
    QHostInfo info= QHostInfo::fromName(getLocalHostName());
    foreach(QHostAddress address,info.addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
             return address.toString();
    }
    return NULL;
}
