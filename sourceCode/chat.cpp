#include "chat.h"
#include "start.h"
#include <QDebug>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QModelIndex>
void Chat::newUI()
{
    glay_left = new QGridLayout;
    glay_right = new QGridLayout;
    tree_view = new QTreeView;
    show_message = new QTextBrowser;
    input_message = new QTextEdit;
    hideOrShow_btn = new QPushButton(tr("显示/隐藏(&H)"));
    send_btn = new QPushButton(tr("发送(&S)"));
    showName_lab = new QLabel(tr("Unknown"));
    hlay = new QHBoxLayout;
}

void Chat::setUI()
{
    glay_left->addWidget(showName_lab,0,0,1,2);
    glay_left->addWidget(show_message,1,0,1,2);
    glay_left->addWidget(input_message,2,0,1,2);
    glay_left->addWidget(hideOrShow_btn,3,0,1,1);
    glay_left->addWidget(send_btn,3,1,1,1);
    glay_right->addWidget(tree_view);
    hlay->addLayout(glay_left);
    hlay->addLayout(glay_right);
    this->setLayout(hlay);
    this->setWindowTitle(tr("简讯"));
}

void Chat::onCloseBtn()
{
    showName_lab->hide();
    show_message->hide();
    input_message->hide();
    hideOrShow_btn->hide();
    send_btn->hide();
}

void Chat::onSendMessage()
{
    sendData.clear();
    sendData.append(input_message->document()->toPlainText());
    quint64 dataSize =  udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,PORT);
    if(dataSize == -1){
        qDebug()<<udpSocket->error();
        qDebug()<<udpSocket->errorString();
    }else{
        qDebug()<<"-------------------------------";
        qDebug()<<"数据报发送成功,字节数="<<dataSize;
        qDebug()<<"sendData="<<sendData;
        qDebug()<<"sendData.data="<<sendData.data();
        qDebug()<<"-------------------------------";
    }
    input_message->clear();
}

void Chat::onSelectUser(const QModelIndex & index)
{
    qDebug()<<"click="<<index.data().toString();
}

void Chat::onReadMessage()
{
    recvData.clear();
    qDebug()<<"准备接收数据";
    while(udpSocket->hasPendingDatagrams()){
        recvData.resize(udpSocket->pendingDatagramSize());
        quint64 dataSize = udpSocket->readDatagram(recvData.data(),recvData.size());
        if(dataSize == -1){
            qDebug()<<udpSocket->errorString();
        }else{
            qDebug()<<"-------------------------------";
            qDebug()<<"数据报接收成功,字节数="<<dataSize;
            qDebug()<<"recvData="<<recvData.data();
            qDebug()<<"-------------------------------";
        }
    }
    show_message->append(recvData.data());
}

void Chat::setConnect()
{
    connect(hideOrShow_btn,SIGNAL(clicked(bool)),this,SLOT(onCloseBtn()));
    connect(tree_view,SIGNAL(entered(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(pressed(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(activated(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
}

void Chat::loadUser()
{
    QSqlQuery sql(sysDB);
    if(sql.exec("select id from user where isOnline = 0;")){
        while(sql.next()){
            treeModel->appendRow(new QStandardItem(tr("%1").arg(sql.value(0).toString())));
        }
        tree_view->setModel(treeModel);
    }else{
        qDebug()<<sql.lastError();
    }
}

void Chat::newSth()
{
    treeModel = new QStandardItemModel(this);
    udpSocket = new QUdpSocket(this);
}

void Chat::init()
{
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("在线人员")<<QStringList("Ip(局域网)")<<QStringList("Pc User"));
    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    if(udpSocket->bind(PORT,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)){
        qDebug()<<"绑定端口成功";
    }else{
        qDebug()<<"绑定端口成功";
    }
}

void Chat::broadCast()
{
    sendData.append("hello!");
    udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,PORT);
}

Chat::Chat(QSqlDatabase & value,QWidget *parent)
    : QWidget(parent)
{
    sysDB = value;
    newUI();
    newSth();
    setUI();
    init();
    setConnect();
    loadUser();
    broadCast();
    show_message->append(tr("ip=%1").arg(getLocalIP()));
    show_message->append(tr("pc name=%1").arg(getUserName()));
}

Chat::~Chat()
{

}

QString Chat::getLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            return address.toString();
        }
    }
    return 0;
}
QString Chat::getUserName()
{
    QStringList envVariables;
    envVariables<<"USERNAME.*"<<"USER.*"<<"USERDOMAIN.*"<<"HOSTNAME.*"<<"DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString str,envVariables) {
        int index = environment.indexOf(QRegExp(str));
        if(index != -1)
        {
            QStringList strlist = (QStringList)environment.at(index).split('=');
            if(strlist.size() == 2){
                return strlist.at(1);
            }
        }
    }
    return "unknown";
}
