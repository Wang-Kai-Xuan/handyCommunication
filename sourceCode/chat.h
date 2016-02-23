#ifndef SERVER_H
#define SERVER_H
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
class Chat : public QWidget
{
    Q_OBJECT

public:
    Chat(QString &Id, QWidget *parent = 0);
    ~Chat();
public:
    QGridLayout * glay_left;
    QGridLayout * glay_right;
    QTreeView * tree_view;
    QTextBrowser * show_message;
    QTextEdit * input_message;
    QPushButton * hideOrShow_btn;
    QPushButton * send_btn;
    QPushButton * close_btn;
    QLabel * message_show_lab;
    QLabel * message_input_lab;
    QHBoxLayout * hlay;
    ILabel * self_id_lab;
    ILabel * about_lab;
    void newUI();
    void setUI();
    void setConnect();
    void loadUser();
    void newSth();
    void init();
    void broadCast(void);
    QString getLocalIP();
    QString getUserName();

    void handleMessage();

    void handleSystemMessage();

    void handleGroupChat();

    void clearSendBuf();

    void clearRecvBuf();

    void setSelfUI(Ioframe* self_info);


    void setSelfConnect(Ioframe* self_info);

private:
    QSqlDatabase sysDB;
    QStandardItemModel *treeModel;
    QUdpSocket * udpSocket;
    QByteArray recvData;
    QByteArray sendData;
    QString userId;
    #define PORT 12060
    bool isNeedDebug;
    QList<QStandardItem *> modelList;

    COMMAND command_send;
    QString content_send;
    COMMAND command_recv;
    QString content_recv;
    QStringList strList;
    Ioframe * self_info;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:
    void onCloseBtn(void);
    void onSendMessage(void);
    void onSelectUser(const QModelIndex &index);
    void onReadMessage(void);
    void onExit(void);
    void onSelf(void);
    void onAbout(void);
    void setSelfName();
};

#endif // SERVER_H
