#include "start.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    Start * w = new Start();
    w->show();
    return a.exec();
}

