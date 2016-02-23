#include "ioframe.h"
#include <QDebug>
void Ioframe::newUI()
{
    vlay = new QVBoxLayout;

    glay = new QGridLayout;
    lab0 = new QLabel(tr("lab0"));
    lab1 = new QLabel(tr("lab1"));
    lab2 = new QLabel(tr("lab2"));
    lab3 = new QLabel(tr("lab3"));
    lab4 = new QLabel(tr("lab4"));
    lab5 = new QLabel(tr("lab5"));
    lab6 = new QLabel(tr("lab6"));
    lab7 = new QLabel(tr("lab7"));
    lab8 = new QLabel(tr("lab8"));

    lin0 = new QLineEdit;
    lin1 = new QLineEdit;
    lin2 = new QLineEdit;
    lin3 = new QLineEdit;
    lin4 = new QLineEdit;
    lin5 = new QLineEdit;
    lin6 = new QLineEdit;
    lin7 = new QLineEdit;
    lin8 = new QLineEdit;

    okBtn = new QPushButton(tr("确认(&Y)"));
    cancelBtn = new QPushButton(tr("取消(&N)"));
}

void Ioframe::setUI()
{
    glay->addWidget(lab0,0,0,1,1);
    glay->addWidget(lin0,0,1,1,1);
    glay->addWidget(lab1,1,0,1,1);
    glay->addWidget(lin1,1,1,1,1);
    glay->addWidget(lab2,2,0,1,1);
    glay->addWidget(lin2,2,1,1,1);
    glay->addWidget(lab3,3,0,1,1);
    glay->addWidget(lin3,3,1,1,1);
    glay->addWidget(lab4,4,0,1,1);
    glay->addWidget(lin4,4,1,1,1);
    glay->addWidget(lab5,5,0,1,1);
    glay->addWidget(lin5,5,1,1,1);
    glay->addWidget(lab6,6,0,1,1);
    glay->addWidget(lin6,6,1,1,1);
    glay->addWidget(lab7,7,0,1,1);
    glay->addWidget(lin7,7,1,1,1);
    glay->addWidget(lab8,8,0,1,1);
    glay->addWidget(lin8,8,1,1,1);
    glay->addWidget(okBtn,9,0,1,1);
    glay->addWidget(cancelBtn,9,1,1,1);
    this->setLayout(glay);
}

void Ioframe::setConnnect()
{
    connect(cancelBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

Ioframe::Ioframe(QWidget *parent) : QWidget(parent)
{
    newUI();
    setUI();
    setConnnect();
}

