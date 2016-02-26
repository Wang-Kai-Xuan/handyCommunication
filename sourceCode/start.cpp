#include "start.h"
#include "menu.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

void Start::setUI()
{
    show_lab = new QLabel(tr("首次登录将会自动注册"));
    show_id = new QLabel(tr("用户名(&U):"));
    show_passwd = new QLabel(tr("用户密码(&P):"));
    lin_id = new QLineEdit;
    lin_passwd = new QLineEdit;
    okBtn = new QPushButton(tr("设置(&S)"));
    cancelBtn = new QPushButton(tr("取消(&C)"));
    glay = new QGridLayout;

    show_id->setBuddy(lin_id);
    show_passwd->setBuddy(lin_passwd);
    lin_passwd->setEchoMode(QLineEdit::Password);

    glay->addWidget(show_lab,0,0,1,2);
    glay->addWidget(show_id,1,0,1,1);
    glay->addWidget(lin_id,1,1,1,1);
    glay->addWidget(show_passwd,2,0,1,1);
    glay->addWidget(lin_passwd,2,1,1,1);
    glay->addWidget(okBtn,3,0,1,1);
    glay->addWidget(cancelBtn,3,1,1,1);

    this->setLayout(glay);
    this->setWindowTitle(tr("登录 -- 局域网通讯工具"));
    this->resize(300,120);
    this->setWindowFlags(Qt::WindowTitleHint);
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
}

void Start::enterChat()
{
    sysDB.close();
    QString id = loginMessage.at(0);
    Menu * chat = new Menu(id);
    chat->show();
    this->close();
}

void Start::allowLogin(QSqlQuery sql)
{
    if(!sql.exec(QString("update user set status = '在线' where id = '%1';").arg(loginMessage.at(0)))){
        QMessageBox msg;
        msg.warning(this,tr("提示"),tr("密码错误，请重试"));
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
    if(sql.exec(QString("select status from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            if(isNeedDebug){
                qDebug()<<"用户状态="<<sql.value(0).toString();
            }
            if(sql.value(0) == "在线"){
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

    if(isRegister){
        if(isOnline){
            QMessageBox msg;
            msg.warning(this,tr("提示"),tr("该用户已登录"));
        }else{
            // 更新状态，isOnline: 1表示在线，0表示不在线
            allowLogin(sql);
        }
    }else{
        if(sql.exec(QString("insert into user(id,name,password,status)values('%1','待用户设定','%2','在线')").arg(loginMessage.at(0)).arg(loginMessage.at(1)))){
            QMessageBox msg;
            msg.information(this,tr("提示"),tr("系统自动帮您注册"));
            enterChat();
        }else{
            qDebug()<<"----------"<<sql.lastError();
            QMessageBox msg;
            msg.critical(this,tr("提示"),tr("该用户编号已被占用，如果不是该用户，可注册其他编号"));
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
    query1.exec("create table user(id text primary key,name text,password text,status text,ip text default 'null',port text default 'null');");
    query1.exec("insert into user(id,name,password,status) values ('root','待用户设定','1111','离线');");
}
