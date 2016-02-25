#ifndef COMMON
#define COMMON
#include <Qt>
#define SEPARATE 0x01
#define PORT 12060
#define CUSTOM_ROLE Qt::UserRole + 1

enum COMMAND{
    COMMAND_NULL,JOIN,GROUP_CHAT,BROADCAST,SECRET_CHAT
};

enum IROLE{
    GROUP,ID,NAME,STATUS
};

#endif // COMMON

