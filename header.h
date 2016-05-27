#ifndef HEADER_H
#define HEADER_H

#include <QComboBox>
#include <ilabel.h>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QKeyEvent>
#include <QEvent>
#include <QWidget>
#include <QDialog>
#include <QStandardPaths>
#include <QProcess>
#include <QLineEdit>
#include <QGridLayout>
#include <QByteArray>
#include <QLabel>
#include <QPicture>
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
#include <QHostInfo>
#include <QInputDialog>
#include <QMediaPlayer>
#include <QMultimedia>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QFileDialog>
#include <QListView>
#include <QDial>
#include <QSlider>
#include <QTimer>
#include "start.h"
#include <QImageReader>

#define SEPARATE "````"
#define CUSTOM_ROLE Qt::UserRole + 1

#define iport 12060

enum COMMAND{
    COMMAND_NULL,BROADCAST_ADDRESS,GROUP_CHAT,BROADCAST,SECRET_CHAT
};

enum IROLE{
    GROUP,ID,NAME,STATUS
};
#endif // HEADER_H

