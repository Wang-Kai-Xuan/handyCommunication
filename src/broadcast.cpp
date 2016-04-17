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
    connect(udpSocket,SIGNAL(readFinished()),this,SLOT(onReadMessage()));
}

void BroadCast::onReadMessage()
{
    QStringList list = udpSocket->getNetWorkContent().split(SEPARATE);
    if(list.at(0) == QString(BROADCAST)){
        show_message->append(QString("<%1>%2").arg(list.at(1)).arg(list.at(2)));
    }
}
void BroadCast::onSendMessage()
{
    send_buf.clear();
    send_buf.append(BROADCAST);
    send_buf.append(SEPARATE);
    send_buf.append(user_id);
    send_buf.append(SEPARATE);
    send_buf.append(input_message->document()->toPlainText());
    udpSocket->sendBroadCast(send_buf);
    input_message->clear();
}

BroadCast::BroadCast(Udp *socket, QString &id, QWidget *parent)
    : QWidget(parent),user_id(id)
{
    udpSocket = socket;
    newUI();
    setUI();
    setConnect();
}
