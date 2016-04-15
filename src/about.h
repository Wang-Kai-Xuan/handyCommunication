#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTextBrowser>
class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);
    QPushButton * okBtn;
    QTextBrowser * bro;
    QGridLayout * vlay;
    void setUI();

    void setConnect();

signals:

public slots:
};

#endif // ABOUT_H
