#include "start.h"
#include "chat.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
void initDB();
QSqlDatabase sysDB;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initDB();
    Start * w = new Start(sysDB);
    w->show();

    return a.exec();
}

void initDB()
{
    sysDB=QSqlDatabase::addDatabase("QSQLITE","connectSystemDB");
    sysDB.setDatabaseName("chat.db");
    if(sysDB.open()){
        qDebug("chat.db已打开");
    }
    else{
        qDebug("chat.db打开出错");
    }

    QSqlQuery query1(sysDB);
    query1.exec("create table user(id text primary key,password text,isOnline integer,ip text default 'null',port text default 'null');");
    query1.exec("insert into user(id,password,isOnline) values ('1','1',0);");
    query1.exec("insert into user(id,password,isOnline) values ('王凯旋','111111',0);");
    query1.exec("insert into user(id,password,isOnline) values ('iuc','123456',0);");
}
