#include "menu.h"
#include "start.h"
#include <QSizePolicy>
#include <QDebug>
#include <QPushButton>
#include <QAction>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QModelIndex>

void Menu::newUI()
{
    glay_menu = new QGridLayout;
    tree_view = new QTreeView;
    close_btn = new QPushButton(tr("退出(&E)"));
    hlay_top = new QHBoxLayout;
    self_info = new Ioframe;
    about_lab = new ILabel;
    broadcast_lab = new ILabel;
    self_id_lab = new ILabel;
}

void Menu::setUI()
{
    hlay_top->addWidget(self_id_lab);
    hlay_top->addWidget(about_lab);
    hlay_top->addWidget(broadcast_lab);

    glay_menu->addLayout(hlay_top,0,0,1,2);
    glay_menu->addWidget(tree_view,1,0,1,2);
    glay_menu->addWidget(close_btn,2,0,1,1);

    about_lab->setText(tr("关于"));
    broadcast_lab->setText(tr("广播"));

    this->setLayout(glay_menu);
    this->setWindowTitle(tr("简讯 - 简单的局域网通讯工具"));
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
    this->resize(300,500);
}

void Menu::enterSecretChat(QString &objId)
{
    SecretLetter * secretLetter = new SecretLetter(userId,objId);
    secretLetter->show();
}

void Menu::enterGroupChat(QString &obj)
{
    GroupChat * groupchat = new GroupChat(obj);
    groupchat->show();
}

void Menu::onSelectUser(const QModelIndex & index)
{
    QString obj;
    QVariant type = index.data(CUSTOM_ROLE);
    switch (type.toInt()){
    case ID:
        obj = index.data().toString();
        enterSecretChat(obj);
        break;
    case NAME:
        qDebug()<<"---NAME---";
        break;
    case STATUS:
        qDebug()<<"---STATUS---";
        break;
    case GROUP:
        obj = index.data().toString();
        enterGroupChat(obj);
        break;
    default:
        break;
    }
}

void Menu::onExit()
{
    QSqlQuery sql(sysDB);
    sql.exec(QString("update user set status = '离线' where id = '%1';").arg(userId));
    this->close();
}

void Menu::setSelfName()
{
    QString str = self_info->lin0->text();
    QSqlQuery sql(sysDB);
    if(!sql.exec(QString("update user set name = '%1' where id = '%2';").arg(str).arg(userId))){
        QMessageBox msg;
        msg.information(this,"提示","抱歉，革命尚未成功，重新再来");
        msg.show();
    }else{
        self_info->close();
        loadGroup();
    }
}

void Menu::onBroadCast()
{
    BroadCast * broadcast = new BroadCast(userId);
    broadcast->show();
}

void Menu::onReadMessage()
{
    QByteArray recvData;
    while(udpSocket->hasPendingDatagrams()){
        recvData.resize(udpSocket->pendingDatagramSize());
        qint64 dataSize = udpSocket->readDatagram(recvData.data(),recvData.size());
        if(dataSize == -1){
            qDebug()<<udpSocket->errorString();
        }
    }
    COMMAND command_recv = (COMMAND)recvData.at(0);
    QString content_recv = recvData.mid(1).data();
    if(command_recv == JOIN){
        QStringList list = content_recv.split(SEPARATE);
        QSqlQuery sql(sysDB);
        if(!sql.exec(QString("update user set ip = '%1' where id = '%2';").arg(list.at(1)).arg(list.at(0)))){
            qDebug()<<"error in recv="<<sql.lastError();
        }
    }
}


void Menu::setSelfConnect(Ioframe* self_info)
{
    connect(self_info->okBtn,SIGNAL(clicked(bool)),this,SLOT(setSelfName()));
}

void Menu::setSelfUI(Ioframe* self_info)
{
    self_info->lab1->hide();
    self_info->lin1->hide();
    self_info->lab2->hide();
    self_info->lin2->hide();
    self_info->lab3->hide();
    self_info->lin3->hide();
    self_info->lab4->hide();
    self_info->lin4->hide();
    self_info->lab5->hide();
    self_info->lin5->hide();
    self_info->lab6->hide();
    self_info->lin6->hide();
    self_info->lab7->hide();
    self_info->lin7->hide();
    self_info->lab8->hide();
    self_info->lin8->hide();
    self_info->setWindowTitle(tr("修改用户名字"));
    self_info->lab0->setText(tr("名字(&M):"));
    self_info->lab0->setBuddy(self_info->lin0);
}

void Menu::onSelf()
{
    setSelfUI(self_info);
    setSelfConnect(self_info);
    setSelfName();
    self_info->show();
}

void Menu::onAbout()
{
    About * about = new About;
    about->show();
}

void Menu::setConnect()
{
    connect(tree_view,SIGNAL(entered(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(pressed(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(activated(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(close_btn,&QPushButton::clicked,this,&Menu::onExit);
    connect(self_id_lab,SIGNAL(click()),this,SLOT(onSelf()));
    connect(about_lab,SIGNAL(click()),this,SLOT(onAbout()));
    connect(broadcast_lab,SIGNAL(click()),this,SLOT(onBroadCast()));
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(onReadMessage()));
}

void Menu::loadGroup()
{
    QSqlQuery sql(sysDB);
    group_list.clear();
    treeModel->clear();
    QStandardItem * group;
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("编号")<<QStringList("姓名")<<QStringList("状态"));
    if(sql.exec("select distinct owner from user;")){
        while(sql.next()){
            group = new QStandardItem(tr("%1").arg(sql.value(0).toString()));
            group->setData(GROUP,CUSTOM_ROLE);
            treeModel->appendRow(group);
            group_list.clear();
        }
    }else{
        qDebug()<<sql.lastError();
    }

    QStandardItem * item;
    QStandardItem * item_id;
    QStandardItem * item_name;
    QStandardItem * item_status;
    QList<QStandardItem *> member_list;
    for(int count = 0; count <= treeModel->columnCount();count++){
        item = treeModel->item(count);
        if(sql.exec(QString("select id,name,status from user where owner = '%1';").arg(treeModel->index(count,0).data().toString()))){
            while(sql.next()){
                item_id = new QStandardItem(QString("%1").arg(sql.value(0).toString()));
                item_name = new QStandardItem(QString("%1").arg(sql.value(1).toString()));
                item_status = new QStandardItem(QString("%1").arg(sql.value(2).toString()));

                item_id->setData(ID,CUSTOM_ROLE);
                item_name->setData(NAME,CUSTOM_ROLE);
                item_status->setData(STATUS,CUSTOM_ROLE);

                member_list.append(item_id);
                member_list.append(item_name);
                member_list.append(item_status);
                item->appendRow(member_list);
                member_list.clear();
            }
        }
    }
    tree_view->setModel(treeModel);
}


void Menu::newSth()
{
    treeModel = new QStandardItemModel(this);
    udpSocket = new QUdpSocket(this);
}

void Menu::init()
{
    self_id_lab->setText(tr("本人ID:%1").arg(userId));
    isNeedDebug =false;
    sysDB=QSqlDatabase::addDatabase("QSQLITE");
    sysDB.setDatabaseName("chat.db");
    if(!sysDB.open()){
        qDebug()<<"数据库打开出错"<<sysDB.lastError();
    }
    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    if(!udpSocket->bind(iport,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)){
        qDebug()<<"绑定端口失败";
    }
}

void Menu::joinBroadCast()
{
    command_send=JOIN;
    content_send.append(userId);
    content_send.append(SEPARATE);
    content_send.append(getLocalIP());
    sendData.append(command_send);
    sendData.append(content_send);
    udpSocket->writeDatagram(sendData.data(),sendData.size(),QHostAddress::Broadcast,iport);
    qDebug()<<"sendData="<<sendData;
}

Menu::Menu(QString &Id, QMainWindow *parent)
    : QMainWindow(parent)
{
//    userId = Id;
//    newUI();
//    newSth();
//    setUI();
//    init();
//    setConnect();
//    loadGroup();
//    joinBroadCast();
}

Menu::~Menu()
{
}

QString Menu::getLocalIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            return address.toString();
        }
    }
    return 0;
}

QString Menu::getUserName()
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

void Menu::closeEvent(QCloseEvent *event)
{
    onExit();
    event->accept();
}

void Menu::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter){
        qDebug()<<"enter";
    }
}
