#include "start.h"
#include "chat.h"
#include <QSqlQuery>
#include <QDebug>
void Start::setUI()
{
    show_lab = new QLabel(tr("首次登录将会自动注册"));
    show_id = new QLabel(tr("用户名"));
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

void Start::check()
{
    QSqlQuery sql(sysDB);
    bool isRegister = false;
    bool isOnline= false;
    qDebug()<<"loginMessage.at(0)="<<loginMessage.at(0);
    qDebug()<<"loginMessage.at(1)="<<loginMessage.at(1);
    if(sql.exec(QString("select password from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            qDebug()<<"password="<<sql.value(0).toString();
            if(sql.value(0) == loginMessage.at(1)){
                isRegister = true;
            }
        }
    }else if(sql.exec(QString("select isOnline from user where id = '%1';").arg(loginMessage.at(0)))){
        while(sql.next()){
            qDebug()<<"isOnline="<<sql.value(0).toString();
            if(sql.value(0) == "1"){
                isOnline= true;
            }
        }
    }
    if(isRegister && !isOnline){
        Chat * chat = new Chat(sysDB);
        chat->show();
        this->close();
    }
}

void Start::onCheck()
{
    loginMessage.append(lin_id->text());
    loginMessage.append(lin_passwd->text());
    if(loginMessage.at(0).isEmpty()&&loginMessage.at(1).isEmpty()){
        show_lab->setText(tr("用户或者密码不能为空"));
    }else{
        check();
    }
}

void Start::setConnect()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onCheck()));
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

Start::Start(QSqlDatabase &value, QWidget *parent) : QWidget(parent)
{
    sysDB = value;
    setUI();
    setConnect();
}

