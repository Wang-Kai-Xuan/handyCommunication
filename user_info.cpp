#include "user_info.h"

void UserInfo::newComponent()
{
    tableWidget = new QTableWidget(this);
    mainlay = new QGridLayout(this);
    name = new QTableWidgetItem("名称");
    content = new QTableWidgetItem("内容");


}

void UserInfo::customLayout()
{
    mainlay->addWidget(tableWidget,0,0,1,1);
    this->setLayout(mainlay);
}

void UserInfo::customTableWidget()
{
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderItem(0,name);
    tableWidget->setHorizontalHeaderItem(1,content);
}

QStringList UserInfo::getUserData()
{
    QSqlQuery sql(sys_db);
    /**
     * @brief user_data
     * 其生存周期为程序开始到结束
     * 其作用域为本代码块
     */
    static QStringList user_data; // 本句只会执行一次
    if(sql.exec(QString("select introduce,sex,phone,email,qq from user where id = %1;").arg(user_id))){
        while(sql.next()){
            for(int i = 0;i < 5;i++){
                user_data.append(sql.value(i).toString());
            }
        }
    }
    return user_data;
}

void UserInfo::loadData()
{
    QStringList data_list = getUserData();
    QStringList head_list;
    head_list <<"姓名"<< "性别" << "电话号码" << "邮箱" << "QQ账户";

    foreach (QString head, head_list) {
        QTableWidgetItem *item = new QTableWidgetItem(head);
        int row = tableWidget->rowCount();
        /*Inserts an empty row into the table at row.*/
        tableWidget->insertRow(row);
        tableWidget->setItem(row,0,item);
    }
    foreach (QString data, data_list) {
        static int row = 0;
        QTableWidgetItem *item = new QTableWidgetItem(data);
        tableWidget->setItem(row,1,item);
        row++;
    }
}

void UserInfo::onItemDataChanged(QTableWidgetItem* item)
{
    QString src;
    switch(item->row()){
    case 0:
        src = "introduce";
        break;
    case 1:
        src = "sex";
        break;
    case 2:
        src = "phone";
        break;
    case 3:
        src = "email";
        break;
    case 4:
        src = "qq";
        break;
    }
//    sql_str = QString("update user set %1 = '%2' where id = '%3';").arg(src).arg(item->text()).arg(user_id);
    QSqlQuery sql(sys_db);
    if(!sql.exec(QString("update user set %1 = '%2' where id = '%3';").arg(src).arg(item->text()).arg(user_id)))
        qDebug()<<sql.lastError();
}

void UserInfo::setConnect()
{
    connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(onItemDataChanged(QTableWidgetItem*)));
}

UserInfo::UserInfo(QSqlDatabase &db, QString &id, QWidget *parent) : sys_db(db),user_id(id),QWidget(parent)
{
    newComponent();
    customTableWidget();
    loadData();
    customLayout();
    setConnect();
}
