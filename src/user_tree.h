#ifndef USER_TREE_H
#define USER_TREE_H

#include <QWidget>
#include "header.h"
#include "udp.h"
class UserTree : public QWidget
{
    Q_OBJECT
public:
    explicit UserTree(QSqlDatabase &db,Udp * socket);
    QStandardItemModel *treeModel;
    QTreeView * tree_view;
    QSqlDatabase &sysDB;
    QList<QStandardItem *> group_list;
    QList<QStandardItem *> member_list;
    QGridLayout *glay;
    QGridLayout *child_glay;
    QTextEdit * msg_input;
    QTextEdit* msg_output;
    QPushButton * hide_btn;
    QPushButton * send_btn;
    Udp * udpSocket;
    void loadGroup();
    void newUI();

    void setUI();

    void setConnect();

    void showMessage();
signals:

public slots:
    void onSelectUser(const QModelIndex &index);
    void onHideMessage();
    void onSendMessage();
    void onRecvMessage();
};

#endif // USER_TREE_H
