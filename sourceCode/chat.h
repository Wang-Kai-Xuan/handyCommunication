#ifndef SERVER_H
#define SERVER_H
#include <QAbstractSocket>
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
    Chat(QSqlDatabase & value,QWidget *parent = 0);
    ~Chat();
public:
    QGridLayout * glay_left;
    QGridLayout * glay_right;
    QTreeView * tree_view;
    QTextBrowser * show_message;
    QTextEdit * input_message;
    QPushButton * hideOrShow_btn;
    QPushButton * send_btn;
    QLabel * showName_lab;
    QHBoxLayout * hlay;

    void newUI();

    void setUI();

    void setConnect();

    void loadUser();

    void newSth();

    void init();

    void broadCast(void);
    QString getLocalIP();
    QString getUserName();
private:
    QSqlDatabase sysDB;
    QStandardItemModel *treeModel;
    QUdpSocket * udpSocket;
    QByteArray recvData;
    QByteArray sendData;
    #define PORT 12060
signals:

public slots:
    void onCloseBtn(void);
    void onSendMessage(void);
    void onSelectUser(const QModelIndex &index);
    void onReadMessage(void);
};

#endif // SERVER_H
