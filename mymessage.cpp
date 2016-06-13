#include "mymessage.h"

MyMessage::MyMessage()
{
    name = new QTableWidgetItem("项目");
    content = new QTableWidgetItem("消息");
    this->setColumnCount(2);
    this->setHorizontalHeaderItem(0,name);
    this->setHorizontalHeaderItem(1,content);
}
