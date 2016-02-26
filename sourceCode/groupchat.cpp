#include "groupchat.h"

GroupChat::GroupChat(QString & group)
{
    group_name = group;
    this->setWindowTitle(tr("群组 %1 通讯 - 简讯").arg(group_name));
    this->setWindowIcon(QIcon(":/new/windowIcon/icon/WindowIcon.png"));
}

