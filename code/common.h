#ifndef COMMON
#define COMMON
#include <Qt>
#define SEPARATE 0x01
#define CUSTOM_ROLE Qt::UserRole + 1

#define iport 12060

enum COMMAND{
    COMMAND_NULL,JOIN,GROUP_CHAT,BROADCAST,SECRET_CHAT
};

enum IROLE{
    GROUP,ID,NAME,STATUS
};

#endif // COMMON

