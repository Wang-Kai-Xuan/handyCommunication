#include "start.h"
#include "menu.h"
#include "broadcast.h"
#include "udp.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include "audioplayer.h"
void openDB(QSqlDatabase &sysDB)
{
    sysDB=QSqlDatabase::addDatabase("QSQLITE","connectSystemDB");
    sysDB.setDatabaseName("handyCommunication.db");
    if(!sysDB.open()){
        qDebug()<<"数据库打开出错"<<sysDB.lastError();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QSqlDatabase sysDB;
    openDB(sysDB);

//    Start * w = new Start(sysDB);
//    w->show();

//    QString id = "2222";
//    Menu * menu = new Menu(id,sysDB);
//    menu->show();

    AudioPlayer audio_player;
    audio_player.show();
    return a.exec();
}

