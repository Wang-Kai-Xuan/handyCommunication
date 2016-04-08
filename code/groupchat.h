#ifndef GROUPCHAT_H
#define GROUPCHAT_H
#include "chatbase.h"

class GroupChat : public ChatBase
{
public:
    GroupChat(QString &group);
    QString group_name;
};

#endif // GROUPCHAT_H
