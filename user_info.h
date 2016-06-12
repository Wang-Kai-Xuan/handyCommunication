#ifndef USER_INFO_H
#define USER_INFO_H

#include <header.h>

class UserInfo : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfo(QSqlDatabase &db,QString &id,QWidget *parent = 0);
    QTableWidget *tableWidget;
    QTableWidgetItem * name;
    QTableWidgetItem * content;
    QGridLayout * mainlay;
    QSqlDatabase &sys_db;
    QString &user_id;
    void newComponent();

    void customLayout();

    void customTableWidget();

    void loadData();

    QStringList getUserData();

    void setConnect();

signals:

public slots:
    void onItemDataChanged(QTableWidgetItem *item);
};

#endif // USER_INFO_H
