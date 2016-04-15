#include "about.h"

void About::setUI()
{
    okBtn = new QPushButton(tr("确认(&C)"));
    vlay = new QGridLayout;
    bro = new QTextBrowser;
    vlay->addWidget(bro,0,0,1,1);
    vlay->addWidget(okBtn,1,0,1,1);
    this->setLayout(vlay);
    this->setWindowTitle(tr("关于 简讯"));
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
}

void About::setConnect()
{
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

About::About(QWidget *parent) : QWidget(parent)
{
    setUI();
    setConnect();
}

