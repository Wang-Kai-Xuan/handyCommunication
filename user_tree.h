#ifndef USER_TREE_H
#define USER_TREE_H

#include <QWidget>
#include "header.h"
#include "udp.h"
class UserTree : public QWidget
{
    Q_OBJECT
private:
    QString show_str_all_user;
    QString show_str_user_tree;
public:
    explicit UserTree(QSqlDatabase &db,QString &id,Udp * socket);
    QStandardItemModel *treeModel;
    QTreeView * tree_view;
    QString &own_id;
    QSqlDatabase &sysDB;
    QList<QStandardItem *> group_list;
    QList<QStandardItem *> member_list;
    QGridLayout *glay;
    QGridLayout *child_glay;
    QGridLayout *child_glay_left;
    QTextEdit * msg_input;
    QTextEdit* msg_output;
    QPushButton * hide_btn;
    QComboBox * view_type;
    QPushButton * send_btn;
    QLabel * chat_type;
    QLabel * chat_id;
    Udp * udpSocket;
    #define BASEROE 1
    void loadGroup();
    void newUI();

    void setUI();

    void setConnect();

    void showMessageUI();
    void groupChat(QString &group_id);
    void sercetChat(QString &recv_user_id);
    QString getUserIP(QString &user_id);
    void loadAllUser();
    void loadMember(QSqlQuery &sql);

    QStringList getGroupMember(QString &group_id);

signals:

public slots:
    void onSelectUser(const QModelIndex &index);
    void onHideMessage();
    void onSendMessage();
    void onRecvMessage();
    void onIndexChangr(QString index);
};

#endif // USER_TREE_H
