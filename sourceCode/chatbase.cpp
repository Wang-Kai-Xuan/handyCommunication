#include "chatbase.h"

void ChatBase::newUI()
{
    glay_main = new QGridLayout;
    show_message = new QTextBrowser;
    input_message = new QTextEdit;
    close_btn = new QPushButton(tr("关闭(&C)"));
    send_btn = new QPushButton(tr("发送(&S)"));
    message_show_lab = new QLabel(tr("消息显示:"));
    message_input_lab = new QLabel(tr("消息输入:"));
    udpSocket = new QUdpSocket(this);
}

void ChatBase::setUI()
{
    glay_main->addWidget(message_show_lab,0,0,1,2);
    glay_main->addWidget(show_message,1,0,1,2);
    glay_main->addWidget(message_input_lab,2,0,1,2);
    glay_main->addWidget(input_message,3,0,1,2);
    glay_main->addWidget(send_btn,4,0,1,1);
    glay_main->addWidget(close_btn,4,1,1,1);

    this->setLayout(glay_main);
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
}

void ChatBase::setConnect()
{
    connect(close_btn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

void ChatBase::init()
{
    if(!udpSocket->bind(PORT,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint)){
        qDebug()<<"绑定端口失败";
    }
}

ChatBase::ChatBase(QWidget *parent) : QWidget(parent)
{
    newUI();
    setUI();
    setConnect();
    init();
}
