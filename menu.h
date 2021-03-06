#ifndef SERVER_H
#define SERVER_H
#include "header.h"
#include "udp.h"
#include "broadcast.h"
#include "user_tree.h"
#include "audioplayer.h"
#include "user_info.h"
#include "mymessage.h"
class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QString &Id,QSqlDatabase &db, QMainWindow *parent = 0);
    ~Menu();
public:
    QMenuBar * menu_bar;
    QMenu *menu_setting;
    QMenu *menu_root;
    QMenu *menu_master;
    QMenu *menu_about;
    /*-------root action-------*/
    QAction *add_group;
    QAction *del_group;
    QAction *alt_group;

    /*-------master action-------*/
    QAction *add_user;
    QAction *del_user;
    QAction *alt_user;

    QAction *about_me;

    QTabWidget * tab_widget;
    BroadCast * widget_broadcast;
    UserTree * user_tree;
    AudioPlayer * audio_player;
    UserInfo * user_info;
//    MyMessage * my_message;

    /*-------old code-------*/
    QHBoxLayout * hlay_top;
    ILabel * about_lab;
    ILabel * broadcast_lab;

    void newComponent();
    void setComponent();
    void setConnect();
    QString getLocalIP();
    QString getUserName();

    void setAction();

    void setTab();

    void addGroup(QStringList &list);

    bool isRoot(QString &id);
    bool isMaster(QString &id);
    void closeEvent(QCloseEvent *event);
    QString getGroupID(QString &master_id);
private:

    Udp* udpSocket;
    QByteArray sendData;
    COMMAND command_send;
    QString content_send;
    QString userId;
    bool isNeedDebug;
    QStringList strList;
public:
    QSqlDatabase &sysDB;

signals:

public slots:

    void onReadMessage(void);
    void onAddGroup();
    void onDelGroup();
    void onAltGroup();
    void onAddUser();
    void onAltUser();
    void onDelUser();
};

#endif // SERVER_H
