#include "chat.h"
#include "start.h"
#include <QSizePolicy>
#include <QDebug>
#include <QPushButton>
#include <QAction>
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
    close_btn = new QPushButton(tr("退出(&E)"));
    message_show_lab = new QLabel(tr("消息显示:"));
    message_input_lab = new QLabel(tr("消息输入:"));
    self_id_lab = new ILabel();
    hlay = new QHBoxLayout;
}

void Chat::setUI()
{
    glay_left->addWidget(message_show_lab,0,0,1,2);
    glay_left->addWidget(show_message,1,0,1,2);
    glay_left->addWidget(message_input_lab,2,0,1,2);
    glay_left->addWidget(input_message,3,0,1,2);

//    glay_left->addWidget(hideOrShow_btn,4,0,1,1);
    glay_left->addWidget(send_btn,4,0,1,1);
    glay_right->addWidget(self_id_lab,0,1,1,2);
    glay_right->addWidget(tree_view,1,1,1,2);
    glay_right->addWidget(close_btn,2,1,1,1);

    hlay->addLayout(glay_left);
    hlay->addLayout(glay_right);
    hlay->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(hlay);
    this->setWindowTitle(tr("简讯"));
    this->setWindowFlags(Qt::WindowTitleHint);
//    this->resize(800,600);
}

void Chat::onCloseBtn()
{
    message_show_lab->hide();
    message_input_lab->hide();
    show_message->hide();
    input_message->hide();
    hideOrShow_btn->hide();
    send_btn->hide();
}

void Chat::clearSendBuf()
{
    sendData.clear();
    command_send = COMMAND_NULL;
    content_send.clear();
}

void Chat::onSendMessage()
{
    clearSendBuf();
    command_send = GROUP_CHAT;
    content_send.append(userId);
    content_send.append(SEPARATE);
    content_send.append(input_message->document()->toPlainText());
    sendData.append(command_send);
    sendData.append(content_send);
    qint64 dataSize =  udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,PORT);
    if(dataSize == -1){
        qDebug()<<"onSendMessage="<<udpSocket->error();
        qDebug()<<"onSendMessage="<<udpSocket->errorString();
    }else{
        if(isNeedDebug){
            qDebug()<<"-------------------------------";
            qDebug()<<"数据报发送成功,字节数="<<dataSize;
            qDebug()<<"sendData="<<sendData;
            qDebug()<<"sendData.data="<<sendData.data();
            qDebug()<<"-------------------------------";
        }
    }
    input_message->clear();
}

void Chat::onSelectUser(const QModelIndex & index)
{
    qDebug()<<"click="<<index.data().toString();
}

void Chat::handleSystemMessage()
{
    show_message->append(QString("<系统消息> 编号:%1用户上线了").arg(content_recv));
    loadUser();
}

void Chat::handleGroupChat()
{
    strList = content_recv.split(SEPARATE);
    show_message->append(QString("<%1> %2").arg(strList.at(0)).arg(strList.at(1)));
}

void Chat::handleMessage()
{
    switch(command_recv){
    case JOIN:
        handleSystemMessage();
        break;
    case GROUP_CHAT:
        handleGroupChat();
        break;
    default:
        break;
    }

}

void Chat::clearRecvBuf()
{
    recvData.clear();
    command_recv = COMMAND_NULL;
    content_recv.clear();
}

void Chat::onReadMessage()
{
    clearRecvBuf();
    while(udpSocket->hasPendingDatagrams()){
        recvData.resize(udpSocket->pendingDatagramSize());
        qint64 dataSize = udpSocket->readDatagram(recvData.data(),recvData.size());
        if(dataSize == -1){
            qDebug()<<udpSocket->errorString();
        }else{
            if(isNeedDebug){
                qDebug()<<"-------------------------------";
                qDebug()<<"数据报接收成功,字节数="<<dataSize;
                qDebug()<<"recvData="<<recvData.data();
                qDebug()<<"-------------------------------";
            }
        }
    }
    command_recv = (COMMAND)recvData.at(0);
    content_recv = recvData.mid(1).data();
    handleMessage();
}

void Chat::onExit()
{
    QSqlQuery sql(sysDB);
    sql.exec(QString("update user set status = '离线' where id = '%1';").arg(userId));
    sysDB.close();
    this->close();
}

void Chat::onSelf()
{
//    Ioframe * self_info = new Ioframe;
//    self_info->show();
}

void Chat::setConnect()
{
    connect(hideOrShow_btn,SIGNAL(clicked(bool)),this,SLOT(onCloseBtn()));
    connect(tree_view,SIGNAL(entered(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(pressed(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(activated(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    connect(close_btn,SIGNAL(clicked(bool)),this,SLOT(onExit()));
    connect(self_id_lab,SIGNAL(click()),this,SLOT(onSelf()));
}

void Chat::loadUser()
{
    QSqlQuery sql(sysDB);
    modelList.clear();
    treeModel->clear();
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("编号")<<QStringList("姓名")<<QStringList("状态"));
    if(sql.exec("select id,name,status from user;")){
        while(sql.next()){
            modelList.append(new QStandardItem(tr("%1").arg(sql.value(0).toString())));
            modelList.append(new QStandardItem(tr("%1").arg(sql.value(1).toString())));
            modelList.append(new QStandardItem(tr("%1").arg(sql.value(2).toString())));
            treeModel->appendRow(modelList);
            modelList.clear();
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
    self_id_lab->setText(tr("本人ID:%1").arg(userId));
    isNeedDebug =false;
    sysDB=QSqlDatabase::addDatabase("QSQLITE");
    sysDB.setDatabaseName("chat.db");
    if(!sysDB.open()){
        qDebug()<<"数据库打开出错"<<sysDB.lastError();

    }
    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    if(!udpSocket->bind(PORT,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)){
        qDebug()<<"绑定端口失败";
    }
}

void Chat::broadCast()
{
    command_send = JOIN;
    content_send = userId;
    sendData.append(command_send);
    sendData.append(content_send);
    udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,PORT);
}

Chat::Chat(QString &Id, QWidget *parent)
    : QWidget(parent)
{
    userId = Id;
    this->setFocus();

    newUI();
    newSth();
    setUI();
    init();
    setConnect();
    loadUser();
    broadCast();
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

void Chat::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == (Qt::Key_Alt && Qt::Key_F4)){
        qDebug()<<"ALT + F4";
//        onExit();
    }
}

void Chat::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == (Qt::Key_Alt && Qt::Key_F4)){
        qDebug()<<"ALT + F4";
//        onExit();
    }
}
