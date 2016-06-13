#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include "header.h"

class MyMessage:public QTableWidget
{
public:
    MyMessage();
    QTableWidgetItem *content;
    QTableWidgetItem *name;
};

#endif // MYMESSAGE_H
