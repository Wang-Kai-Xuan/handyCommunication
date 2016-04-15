#include "start.h"
#include "menu.h"
#include "chatbase.h"
#include "broadcast.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
//    Start * w = new Start();
//    w->show();
    QString id = "999";
    Menu * menu = new Menu(id);
    menu->show();
    return a.exec();
}

