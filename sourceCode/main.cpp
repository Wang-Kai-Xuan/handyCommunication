#include "start.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Start * w = new Start();
    w->show();
    return a.exec();
}

