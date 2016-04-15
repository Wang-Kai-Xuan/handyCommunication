#ifndef SERVER_H
#define SERVER_H
#include "groupchat.h"
#include "secretletter.h"
#include "broadcast.h"
#include "about.h"
#include "common.h"
#include <ioframe.h>
#include <ilabel.h>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QKeyEvent>
#include <QEvent>
#include <QWidget>
#include <QDialog>
#include <QProcess>
#include <QLineEdit>
#include <QGridLayout>
#include <QByteArray>
#include <QLabel>
#include <QSqlDatabase>
#include <QPushButton>
#include <QTreeView>
#include <QTextBrowser>
#include <QTextEdit>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QMainWindow>
class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QString &Id, QMainWindow *parent = 0);
    ~Menu();
public:
    QGridLayout * glay_menu;
    QTreeView * tree_view;
    QPushButton * close_btn;
    QHBoxLayout * hlay_top;
    ILabel * about_lab;
    ILabel * broadcast_lab;
    ILabel * self_id_lab;
    void newUI();
    void setUI();
    void setConnect();
    void loadGroup();
    void newSth();
    void init();
    void joinBroadCast(void);
    QString getLocalIP();
    QString getUserName();
    void setSelfUI(Ioframe* self_info);
    void setSelfConnect(Ioframe* self_info);
    void enterSecretChat(QString &objId);
    void enterGroupChat(QString &obj);

private:
    QSqlDatabase sysDB;
    QStandardItemModel *treeModel;
    QUdpSocket * udpSocket;
    QByteArray sendData;
    COMMAND command_send;
    QString content_send;
    QString userId;
    bool isNeedDebug;
    QList<QStandardItem *> group_list;
    QList<QStandardItem *> member_list;
    QStringList strList;
    Ioframe * self_info;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void onSelectUser(const QModelIndex &index);
    void onExit(void);
    void onSelf(void);
    void onAbout(void);
    void setSelfName();
    void onBroadCast(void);
    void onReadMessage(void);
};

#endif // SERVER_H
