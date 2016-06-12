#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "header.h"
#include "start.h"
#include "menu.h"
void Start::setUI()
{
    show_lab = new QLabel(tr("首次登录将会自动注册\n用户编号为4位数字\n用户密码不少于4位，包括大小写字母、数字、下划线"));
    show_id = new QLabel(tr("用户编号(&U):"));
    show_passwd = new QLabel(tr("用户密码(&P):"));
    lin_id = new QLineEdit;
    lin_passwd = new QLineEdit;
    okBtn = new QPushButton(tr("设置(&L)"));
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

void Start::enterMenu(QSqlQuery & sql)
{
    QString id = loginMessage.at(0);
    Menu * chat = new Menu(id,sysDB);
    chat->show();
    this->close();
}

void Start::check()
{
    QSqlQuery sql(sysDB);
    bool isRegister = false;
    if(sql.exec(QString("select password from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            if(sql.value(0).toString() == loginMessage.at(1)){
                isRegister = true;
                break;
            }
        }
    }

    if(isRegister){
        if(sql.exec(QString("update user set isOnline = '在线' where id = '%1';").arg(loginMessage.at(0)))){
            enterMenu(sql);
        }else{
            qDebug()<<sql.lastError()<<endl;
        }
    }else{
        if(sql.exec(QString("insert into user(id,password,isOnline)values('%1','%2','在线')")\
                    .arg(loginMessage.at(0)).arg(loginMessage.at(1)))){
            if(sql.exec(QString("create table user_history_%1(sender text not null,content text default 'null',time text default current_timestamp);")\
                .arg(loginMessage.at(0)))){
                QMessageBox::information(this,tr("提示"),tr("系统自动帮您注册"));
                enterMenu(sql);
            }
            qDebug()<<sql.lastError()<<endl;
        }
    }
}

void Start::onCheck()
{
    loginMessage.clear();
    loginMessage.append(lin_id->text());
    loginMessage.append(lin_passwd->text());

    if((loginMessage.at(0).isEmpty())&&(loginMessage.at(1).isEmpty())){
        QMessageBox ::critical(this,tr("提示"),tr("用户或者密码不能为空"),"确定");
    }else{
        QRegExp reg_id("^\\d\\d\\d\\d$");
        QRegExp reg_pwd("^\\w\\w\\w\\w");
        if((reg_id.indexIn(loginMessage.at(0)) == 0) && (reg_pwd.indexIn(loginMessage.at(1)) == 0)){
            check();
        }else{
            QMessageBox ::warning(this,tr("提示"),tr("格式错误"),"确定");
        }
    }
    lin_id->clear();
    lin_passwd->clear();
}

void Start::setConnect()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onCheck()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

Start::Start(QSqlDatabase &db, QWidget *parent)
    : QWidget(parent),sysDB(db)
{
    isNeedDebug = false;
    setUI();
    initDB();
    setConnect();
}

void Start::initDB()
{
    QSqlQuery query1(sysDB);

    QString str = "create table user(id text primary key,password text not null\
,phone text default '未设置',email text default '未设置',isRoot text default 'no',qq text default '未设置'\
,sex text default '未设置',introduce text default '未设置',isOnline text default '未设置'\
,ip text default 'null',port text default 'null',registerTime text default current_timestamp\
,localHostName text default 'null');";
    if(!query1.exec(str)){
        qDebug()<<query1.lastError();
    }

    str = "create table _group(id text primary key,master_id text not null,introduce text default 'null',buildTime text default current_timestamp);";
    if(!query1.exec(str)){
        qDebug()<<query1.lastError();
    }
    str = "create table broadcast_history(sender text not null,content text default 'null',time text default current_timestamp);";
    if(!query1.exec(str)){
        qDebug()<<query1.lastError();
    }

    if(!query1.exec("insert into user(id,password,sex,isOnline,isRoot) values ('0000','0000','男','离线','yes');")){
        qDebug()<<query1.lastError();
    }
}
