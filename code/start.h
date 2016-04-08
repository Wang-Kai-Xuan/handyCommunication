#ifndef FIRSTUSE_H
#define FIRSTUSE_H
#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QByteArray>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>

class Start : public QWidget
{
    Q_OBJECT
public:
    QVector<QString> loginMessage;
    QLabel * show_lab;
    QLabel * show_id;
    QLabel * show_passwd;
    QLineEdit * lin_id;
    QLineEdit * lin_passwd;
    QPushButton * okBtn;
    QPushButton * cancelBtn;
    QGridLayout * glay;
public:
    QSqlDatabase sysDB;
    bool isNeedDebug;
public:
    explicit Start(QWidget *parent = 0);
    void setUI();
    void setConnect();
    void check();
    void allowLogin(QSqlQuery sql);
    void enterChat();
    void initDB();
signals:

public slots:
    void onCheck(void);
};

#endif // FIRSTUSE_H
