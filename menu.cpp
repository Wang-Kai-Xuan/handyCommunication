#include "menu.h"

void Menu::newComponent()
{
    udpSocket = new Udp();
    menu_bar = new QMenuBar(this);
    menu_setting= new QMenu("设置(&S)");
    menu_root= new QMenu("root功能(&R)");
    menu_master= new QMenu("组长功能(&M)");

    /*QTabWidget*/
    tab_widget = new QTabWidget();
    widget_broadcast = new BroadCast(sysDB,udpSocket,userId,this);
    user_tree = new UserTree(sysDB,userId,udpSocket);
    audio_player = new AudioPlayer();
    user_info= new UserInfo(sysDB,userId);
    my_message= new MyMessage();

    /*root action*/
    add_group = new QAction("添加组",menu_root);
    del_group = new QAction("删除组",menu_root);
    alt_group = new QAction("修改组",menu_root);
    /*master action*/
    add_user= new QAction("添加用户",menu_master);
    del_user= new QAction("删除用户",menu_master);
    alt_user= new QAction("修改用户",menu_master);
}

void Menu::setAction()
{
    menu_setting->addAction("主题(&T)");
    menu_setting->addAction("字题(&F)");

    menu_root->addAction(add_group);
    menu_root->addAction(del_group);
    menu_root->addAction(alt_group);

    menu_master->addAction(add_user);
    menu_master->addAction(del_user);
    menu_master->addAction(alt_user);

    menu_bar->addMenu(menu_setting);
    menu_bar->addMenu(menu_root);
    menu_bar->addMenu(menu_master);
}

void Menu::setTab()
{
    tab_widget->addTab(my_message,"我的消息");
    tab_widget->addTab(widget_broadcast,"广播");
    tab_widget->addTab(user_tree,"通讯列表");
    tab_widget->addTab(audio_player,"播放音频");
    tab_widget->addTab(user_info,"本人信息及设置");
    tab_widget->setTabPosition(QTabWidget::West);
    tab_widget->setTabShape(QTabWidget::Rounded);
    tab_widget->setStyleSheet("font: 14pt \"宋体\";");
}

void Menu::setComponent()
{
    setAction();
    setTab();
    this->setMenuBar(menu_bar);
    this->setCentralWidget(tab_widget);
    this->setWindowTitle(tr("简讯 - 简单的局域网通讯工具"));
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
    this->resize(800,600);
}


bool Menu::isRoot(QString &id)
{
    QSqlQuery sql(sysDB);
    if(sql.exec(QString("select isRoot from user where id = '%1';").arg(id))){
        while(sql.next()){
            if(sql.value(0) == "yes"){
                return true;
            }
        }
        return false;
    }else
        return false;
}

bool Menu::isMaster(QString &id)
{
    QSqlQuery sql(sysDB);
    if(sql.exec("select master_id from _group;")){
        while(sql.next()){
            if(sql.value(0) == id){
                return true;
            }
        }
        return false;
    }else
        return false;
}

QString Menu::getGroupID(QString &master_id)
{
    static QString group_id;
    QSqlQuery sql(sysDB);
    if(sql.exec(QString("select id from _group where master_id = '%1';").arg(master_id))){
        while(sql.next()){
            group_id=sql.value(0).toString();
            }
        return group_id;
    }else
        return NULL;
}

void Menu::addGroup(QStringList &list)
{
    QSqlQuery sql(sysDB);
    bool isOk = true;
    if(!sql.exec(QString("insert into _group(id,master_id,introduce) values ('%1','%2','%3');").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)))){
        qDebug()<<sql.lastError()<<endl;
        isOk = false;
    }
    if(!sql.exec(QString("create table _group_member_%1(member_id text primary key);").arg(list.at(0)))){
        qDebug()<<sql.lastError()<<endl;
        isOk = false;
    }

    if(!sql.exec(QString("create table _group_history_%1(sender text not null,content text default 'null',time text default current_timestamp);").arg(list.at(0)))){
        qDebug()<<sql.lastError()<<endl;
        isOk = false;
    }

    if(!isOk){
        QMessageBox::warning(this, tr("提示"),tr("增加组失败"),QMessageBox::Ok);
    }
}

void Menu::onAddGroup()
{
    if(!isRoot(userId)){
        QMessageBox::warning(this, tr("提示"),tr("需要Root权限！"),QMessageBox::Ok);
        return ;
    }
    bool ok;
    QString text = QInputDialog::getText(this\
                                         ,tr("增加组")\
                                         ,tr("组ID 管理员ID 说明(空格分开)")\
                                         ,QLineEdit::Normal\
                                         ,NULL, &ok);

    if (ok && !text.isEmpty()){
        QStringList list = text.split(' ');
        if(list.size() == 3){
            addGroup(list);
        }
    }else{
        QMessageBox::warning(this, tr("提示"),tr("输入格式有误，请重新输入！"),QMessageBox::Ok);
    }
    user_tree->loadGroup();
}

void Menu::onDelGroup()
{
    /*获取需要删除的组ID*/
    bool ok;
    QSqlQuery sql(sysDB);
    QString text = QInputDialog::getText(this\
                                         ,tr("删除组")\
                                         ,tr("组ID")\
                                         ,QLineEdit::Normal\
                                         ,NULL, &ok);

    if (!ok && text.isEmpty()){
        QMessageBox::warning(this, tr("提示"),tr("输入格式有误，请重新输入！"),QMessageBox::Ok);
        return;
    }
    /*在group里面删除组ID及其管理员*/
    if(!sql.exec(QString("delete from _group where id = %1;").arg(text))){
        qDebug()<<"onDelGroup"<<sql.lastError()<<endl;
    }
    /*删除成员表*/
    if(!sql.exec(QString("drop table _group_member_%1;").arg(text))){
        qDebug()<<"onDelGroup"<<sql.lastError()<<endl;
    }

    /*删除在组聊天记录表*/
    if(!sql.exec(QString("drop table _group_history_%1;").arg(text))){
        qDebug()<<"onDelGroup"<<sql.lastError()<<endl;
    }
}

void Menu::onAltGroup()
{

}
void Menu::onAddUser()
{
    QSqlQuery sql(sysDB);
    if(!isMaster(userId)){
        QMessageBox::warning(this, tr("提示"),tr("需要管理员权限！"),QMessageBox::Ok);
        return ;
    }
    bool ok;
    QString text = QInputDialog::getText(this\
                                         ,tr("增加用户")\
                                         ,tr("用户ID")\
                                         ,QLineEdit::Normal\
                                         ,NULL, &ok);


    if (ok && !text.isEmpty()){
        if(!sql.exec(QString("insert into _group_member_%1(member_id) values ('%2');")\
                     .arg(getGroupID(userId)).arg(text))){
            qDebug()<<sql.lastError()<<endl;
            QMessageBox::warning(this, tr("提示"),tr("增加用户失败"),QMessageBox::Ok);
        }
    }else{
        QMessageBox::warning(this, tr("提示"),tr("输入格式有误，请重新输入！"),QMessageBox::Ok);
    }

    user_tree->loadGroup();
}

void Menu::onAltUser()
{

}

void Menu::onDelUser()
{

}

void Menu::onReadMessage()
{
    QStringList list = udpSocket->getNetWorkContent().split(SEPARATE);
    if(list.at(0) != QString(BROADCAST_ADDRESS)) return ;
    if(list.size() != 3) return ;
    QSqlQuery sql(sysDB);
    if(!sql.exec(QString("update user set ip = '%1' where id = '%2';").arg(list.at(2)).arg(list.at(1))))
        qDebug()<<sql.lastError();
}

void Menu::setConnect()
{
    connect(add_group,SIGNAL(triggered(bool)),this,SLOT(onAddGroup()));
    connect(del_group,SIGNAL(triggered(bool)),this,SLOT(onDelGroup()));
    connect(alt_group,SIGNAL(triggered(bool)),this,SLOT(onAltGroup()));
    connect(add_user,SIGNAL(triggered(bool)),this,SLOT(onAddUser()));
    connect(del_user,SIGNAL(triggered(bool)),this,SLOT(onDelUser()));
    connect(alt_user,SIGNAL(triggered(bool)),this,SLOT(onAltUser()));
    connect(udpSocket,SIGNAL(readSecretMessage()),this,SLOT(onReadMessage()));
}

Menu::Menu(QString &Id, QSqlDatabase &db, QMainWindow *parent)
    : QMainWindow(parent),userId(Id),sysDB(db)
{
    newComponent();
    setComponent();
    setConnect();
    udpSocket->send(QString(BROADCAST_ADDRESS)+QString(SEPARATE+userId+SEPARATE+udpSocket->getLocalIP()));
    this->setTabShape(QTabWidget::Triangular);
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
    QSqlQuery sql(sysDB);
    if(!sql.exec(QString("update user set isOnline = '离线' where id = '%1';").arg(userId))){
        qDebug()<<sql.lastError()<<endl;
    }
    sysDB.close();
}
