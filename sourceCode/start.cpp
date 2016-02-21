#include "start.h"
#include "chat.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
void Start::setUI()
{
    show_lab = new QLabel(tr("首次登录将会自动注册"));
    show_id = new QLabel(tr("用户名:"));
    show_passwd = new QLabel(tr("用户密码:"));
    lin_id = new QLineEdit;
    lin_passwd = new QLineEdit;
    okBtn = new QPushButton(tr("设置(&S)"));
    cancelBtn = new QPushButton(tr("取消(&C)"));
    glay = new QGridLayout;

    glay->addWidget(show_lab,0,0,1,2);
    glay->addWidget(show_id,1,0,1,1);
    glay->addWidget(lin_id,1,1,1,1);
    glay->addWidget(show_passwd,2,0,1,1);
    glay->addWidget(lin_passwd,2,1,1,1);
    glay->addWidget(okBtn,3,0,1,1);
    glay->addWidget(cancelBtn,3,1,1,1);

    this->setLayout(glay);
    this->setWindowTitle(tr("提示"));
    this->setWindowFlags(Qt::WindowTitleHint);
}

void Start::enterChat()
{
//    QSqlDatabase::removeDatabase("connectSystemDB");
    sysDB.close();
    QString id = loginMessage.at(0);
    Chat * chat = new Chat(id);
    chat->show();

    this->close();
}

void Start::allowLogin(QSqlQuery sql)
{
    if(!sql.exec(QString("update user set isOnline = 1 where id = '%1';").arg(loginMessage.at(0)))){
        QMessageBox msg;
        msg.critical(this,tr("提示"),tr("出错了，请重试"));
    }else{
        enterChat();
    }
}

void Start::check()
{
    QSqlQuery sql(sysDB);
    bool isRegister = false;
    bool isOnline= false;
    if(isNeedDebug){
        qDebug()<<"loginMessage.at(0)="<<loginMessage.at(0);
        qDebug()<<"loginMessage.at(1)="<<loginMessage.at(1);
    }
    if(sql.exec(QString("select password from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            if(isNeedDebug){
                qDebug()<<"用户密码="<<sql.value(0).toString();
            }
            if(sql.value(0) == loginMessage.at(1)){
                isRegister = true;
            }else{
                isRegister = false;
            }
        }
    }
    if(sql.exec(QString("select isOnline from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            if(isNeedDebug){
                qDebug()<<"用户状态="<<sql.value(0).toString();
            }
            if(sql.value(0) == 1){
                isOnline= true;
            }else{
                isOnline= false;
            }
        }
    }
    if(isNeedDebug){
        qDebug()<<"isRegister="<<isRegister;
        qDebug()<<"isOnline="<<isOnline;
    }
    // 如果已经注册且没有在线，则可以登录
    if(isRegister){
        if(isOnline){
            QMessageBox msg;
            msg.critical(this,tr("提示"),tr("该用户已登录"));
        }else{
            // 更新状态，isOnline: 1表示在线，0表示不在线
            allowLogin(sql);
        }
    }else{
        if(sql.exec(QString("insert into user(id,password,isOnline)values(%1,%2,1)").arg(loginMessage.at(0)).arg(loginMessage.at(1)))){
            QMessageBox msg;
            msg.critical(this,tr("提示"),tr("系统自动帮您注册"));
            enterChat();
        }else{
            qDebug()<<sql.lastError();
            QMessageBox msg;
            msg.critical(this,tr("提示"),tr("系统无法自动帮您注册"));
            this->close();
        }
    }
}

void Start::onCheck()
{
    loginMessage.clear();
    loginMessage.append(lin_id->text());
    loginMessage.append(lin_passwd->text());
    if(loginMessage.at(0).isEmpty()&&loginMessage.at(1).isEmpty()){
        QMessageBox msg;
        msg.critical(this,tr("提示"),tr("用户或者密码不能为空"),"确定");
    }else{
        check();
    }
    lin_id->clear();
    lin_passwd->clear();
}

void Start::setConnect()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onCheck()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

Start::Start(QWidget *parent) : QWidget(parent)
{
    isNeedDebug = false;
    setUI();
    initDB();
    setConnect();

}
void Start::initDB()
{
    sysDB=QSqlDatabase::addDatabase("QSQLITE","connectSystemDB");
    sysDB.setDatabaseName("chat.db");
    if(!sysDB.open()){
        qDebug()<<"数据库打开出错"<<sysDB.lastError();
    }
    QSqlQuery query1(sysDB);
    query1.exec("create table user(id text primary key,password text,isOnline integer,ip text default 'null',port text default 'null');");
    query1.exec("insert into user(id,password,isOnline) values ('root','1111',0);");
}
