#ifndef ChatBase_H
#define ChatBase_H
#include "ilabel.h"
#include <QWidget>
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
#include "common.h"
class ChatBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBase(QWidget *parent = 0);
public:
    QGridLayout * glay_main;
    QTextBrowser * show_message;
    QTextEdit * input_message;
    QPushButton * close_btn;
    QPushButton * send_btn;
    QLabel * message_show_lab;
    QLabel * message_input_lab;

    void newUI();
    void setUI();
    void setConnect();

public:
    QUdpSocket * udpSocket;

    void init();


signals:

public slots:
};

#endif // ChatBase_H
