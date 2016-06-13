#include "broadcast.h"

void BroadCast::newUI()
{
    glay_main = new QGridLayout;
    show_message = new QTextBrowser;
    input_message = new QTextEdit;
    send_btn = new QPushButton(tr("发送(&D)"));
    message_show_lab = new QLabel(tr("消息显示:"));
    message_input_lab = new QLabel(tr("消息输入:"));
}

void BroadCast::setUI()
{
    glay_main->addWidget(message_show_lab,0,0,1,2);
    glay_main->addWidget(show_message,1,0,1,2);
    glay_main->addWidget(message_input_lab,2,0,1,2);
    glay_main->addWidget(input_message,3,0,1,2);
    glay_main->addWidget(send_btn,4,0,1,1);

    this->setLayout(glay_main);
}

void BroadCast::setConnect()
{
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    connect(udpSocket,SIGNAL(readBroadCastMessage()),this,SLOT(onReadMessage()));
}

void BroadCast::onReadMessage()
{
    QStringList list = udpSocket->getNetWorkContent().split(SEPARATE);
    if(list.at(0) == QString(BROADCAST)){

        if(list.at(1) == user_id){
            show_message->setTextColor(QColor("blue"));
            show_message->append(QString("%1").arg(list.at(2)));
            show_message->setAlignment(Qt::AlignRight);
        }else{
            show_message->setTextColor(QColor("black"));
            show_message->append(QString("<%1>%2").arg(list.at(1)).arg(list.at(2)));
            show_message->setAlignment(Qt::AlignLeft);

        }
        QSqlQuery sql(sysDB);
        if(!sql.exec(QString("insert into broadcast_history(sender,content) values ('%1','%2');")\
                     .arg(list.at(1)).arg(list.at(2)))){
            qDebug()<<sql.lastError();
        }
    }
}
void BroadCast::onSendMessage()
{
    udpSocket->send(QString(BROADCAST)+QString(SEPARATE+user_id+SEPARATE+input_message->document()->toPlainText()));
    input_message->clear();
}

BroadCast::BroadCast(QSqlDatabase &db, Udp *socket, QString &id, QWidget *parent)
    : QWidget(parent),user_id(id),sysDB(db)
{
    udpSocket = socket;
    newUI();
    setUI();
    setConnect();
}
