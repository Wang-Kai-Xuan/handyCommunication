#ifndef IOFRAME_H
#define IOFRAME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTcpSocket>

class Ioframe : public QWidget
{
    Q_OBJECT
public:
    explicit Ioframe(QWidget *parent = 0);
public:
    void newUI();
    void setUI();
    void setConnnect();
private:

public:
    QVBoxLayout * vlay;
    QGridLayout * glay;
    QLabel * lab0;
    QLabel * lab1;
    QLabel * lab2;
    QLabel * lab3;
    QLabel * lab4;
    QLabel * lab5;
    QLabel * lab6;
    QLabel * lab7;
    QLabel * lab8;
    QLineEdit * lin0;
    QLineEdit * lin1;
    QLineEdit * lin2;
    QLineEdit * lin3;
    QLineEdit * lin4;
    QLineEdit * lin5;
    QLineEdit * lin6;
    QLineEdit * lin7;
    QLineEdit * lin8;

    QPushButton * okBtn;
    QPushButton * cancelBtn;
public slots:

};

#endif // IOFRAME_H
