#ifndef HEADER_H
#define HEADER_H

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
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTabWidget>
#include <QSizePolicy>
#include <QDebug>
#include <QAction>
#include <QSqlQuery>
#include <QSqlError>
#include <QModelIndex>
#include <QInputDialog>
#include "start.h"
#include "about.h"

#define SEPARATE 0x01
#define CUSTOM_ROLE Qt::UserRole + 1

#define iport 12060

enum COMMAND{
    COMMAND_NULL,JOIN,GROUP_CHAT,BROADCAST,SECRET_CHAT
};

enum IROLE{
    GROUP,ID,NAME,STATUS
};
#endif // HEADER_H

