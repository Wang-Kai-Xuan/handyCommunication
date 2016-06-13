#include "user_tree.h"

void UserTree::newUI()
{
    tree_view = new QTreeView();
    treeModel = new QStandardItemModel;
    glay = new QGridLayout;
    child_glay = new QGridLayout;
    child_glay_left = new QGridLayout;
    msg_input = new QTextEdit;
    msg_output = new QTextEdit;
    hide_btn = new QPushButton("隐藏(&H)");
    send_btn = new QPushButton("发送(&S)");
    chat_type = new QLabel("...");
    chat_id= new QLabel("...");
    view_type = new QComboBox;
}

void UserTree::setUI()
{
    view_type->addItem(show_str_user_tree);
    view_type->addItem(show_str_all_user);
    view_type->setFixedWidth(100);

    child_glay->addWidget(chat_type,0,0,1,1);
    child_glay->addWidget(chat_id,0,1,1,1);
    child_glay->addWidget(msg_output,BASEROE,0,1,2);
    child_glay->addWidget(msg_input,BASEROE+1,0,1,2);
    child_glay->addWidget(hide_btn,BASEROE+2,0,1,1);
    child_glay->addWidget(send_btn,BASEROE+2,1,1,1);

    child_glay_left->addWidget(view_type,0,0,1,1);
    child_glay_left->addWidget(tree_view,1,0,1,1);

    glay->addLayout(child_glay_left,0,0);
    glay->addLayout(child_glay,0,1);

    this->setLayout(glay);

    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    msg_output->setStyleSheet("font: 12pt \"华文行楷\";");
}

void UserTree::setConnect()
{
    connect(tree_view,SIGNAL(entered(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(activated(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(clicked(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(hide_btn,SIGNAL(clicked(bool)),this,SLOT(onHideMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    connect(udpSocket,SIGNAL(readSecretMessage()),this,SLOT(onRecvMessage()));
    connect(view_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(onIndexChangr(QString)));
}

void UserTree::onHideMessage()
{
    glay->setColumnStretch(0,1);
    glay->setColumnStretch(1,0);

    chat_id->hide();
    chat_type->hide();
    msg_input->hide();
    msg_output->hide();
    hide_btn->hide();
    send_btn->hide();
}

void UserTree::onSendMessage()
{
    QString id = chat_id->text();
    if(chat_type->text() == "私信->"){
        sercetChat(id);
    }
    if(chat_type->text() == "组播->"){
        groupChat(id);
    }
}

void UserTree::onIndexChangr(QString index)
{
    qDebug()<<"index="<<index;
    if(index == show_str_user_tree){
        loadGroup();
    }
    if(index == show_str_all_user){
        loadAllUser();
    }

}

void UserTree::onRecvMessage()
{
    QStringList list =  udpSocket->getNetWorkContent().split(SEPARATE);
    foreach (QString str, list) {
        qDebug()<<"str="<<str;
    }

    if(list.size() != 4) return ;
    if(list.at(2) != own_id)return ;
    msg_output->setTextColor(QColor("black"));
    msg_output->append(list.at(3));
    msg_output->setAlignment(Qt::AlignLeft);
}

void UserTree::showMessageUI()
{
    glay->setColumnStretch(0,1);
    glay->setColumnStretch(1,2);

    chat_id->show();
    chat_type->show();
    msg_input->show();
    msg_output->show();
    hide_btn->show();
    send_btn->show();
}

QStringList UserTree::getGroupMember(QString & group_id)
{
    static QStringList list;
    QSqlQuery sql(sysDB);
    if(sql.exec(QString("select member_id from _group_member_%1;").arg(group_id))){
        while(sql.next()){
            list.append(sql.value(0).toString());
        }
    }
    return list;
}

void UserTree::groupChat(QString & group_id)
{
    /*获取组的成员名单*/
    QStringList list = getGroupMember(group_id);
    foreach (QString str, list) {
        qDebug()<<"str="<<str;
    }
    /*向每个成员发送消息*/
    /*添加组的聊天历史*/
}

QString UserTree::getUserIP(QString & user_id)
{
    static QString ip;
    QSqlQuery sql(sysDB);
    if(sql.exec(QString("select ip from user where id = '%1';").arg(user_id))){
        while(sql.next()){
            ip = sql.value(0).toString();
            return ip;
        }
    }else{
        qDebug()<<sql.lastError();
        return NULL;
    }

}
void UserTree::sercetChat(QString & user_id)
{    
    msg_output->setTextColor(QColor("blue"));
    msg_output->append(msg_input->document()->toPlainText());
    msg_output->setAlignment(Qt::AlignRight);

    udpSocket->send(QString(SECRET_CHAT)+\
                    QString(SEPARATE+own_id)+\
                    QString(SEPARATE+user_id)+\
                    QString(SEPARATE+msg_input->document()->toPlainText()));
    msg_input->clear();
}

void UserTree::onSelectUser(const QModelIndex &index)
{
    QString item_id;
    QVariant type = index.data(CUSTOM_ROLE);

    switch(type.toInt()){
    case USER:
        item_id = index.data().toString();
        showMessageUI();
        chat_type->setText("私信->");
        chat_id->setText(item_id);
        break;

    case GROUP:
        item_id = index.data().toString();
        groupChat(item_id);
        chat_type->setText("组播->");
        chat_id->setText(item_id);
        break;
    }
}

UserTree::UserTree(QSqlDatabase &db, QString &id, Udp *socket):sysDB(db),own_id(id)
{
    udpSocket = socket;
    show_str_all_user = "所有用户";
    show_str_user_tree = "用户树";
    newUI();
    setUI();
    loadGroup();
    setConnect();
    onHideMessage();
    this->setStyleSheet("font: 12pt \"宋体\";");
}

void UserTree::loadMember(QSqlQuery &sql)
{
    QStandardItem * group;
    QStandardItem * member_id;
    QList<QStandardItem *>  item_list;
    QSqlQuery &sql1 = sql;
    for(int count = 0; count < treeModel->rowCount();count++){
        group = treeModel->item(count);
        if(sql.exec(QString("select member_id from _group_member_%1;").arg(treeModel->index(count,0).data().toString()))){
            while(sql.next()){
                member_id = new QStandardItem(QString("%1").arg(sql.value(0).toString()));
                member_id->setData(USER,CUSTOM_ROLE);
                item_list.append(member_id);
                item_list.append(new QStandardItem("   "));
                if(sql1.exec(QString("select introduce from user where id=%1;").arg(sql.value(0).toString()))){
                    while(sql1.next()){
                        item_list.append(new QStandardItem(QString("%1").arg(sql1.value(0).toString())));
                    }
                }
                group->appendRow(item_list);
                item_list.clear();
            }
        }
    }
}

void UserTree::loadGroup()
{
    QSqlQuery sql(sysDB);
    group_list.clear();
    treeModel->clear();
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("ID")<<QStringList("组长")<<QStringList("说明"));
    if(sql.exec("select count(*) from user;")){
        while(sql.next()){
            qint8 num = sql.value(0).toInt();
            if(num == 0)QMessageBox::information(this,tr("提示"),tr("没有设置分组"));
        }
    }


    QList<QStandardItem *> group_list;
    QStandardItem * group;
    QStandardItem * master;
    QStandardItem * introduce;
    if(sql.exec("select id,master_id,introduce from _group;")){
        while(sql.next()){
            group = new QStandardItem(tr("%1").arg(sql.value(0).toString()));
            master = new QStandardItem(tr("%1").arg(sql.value(1).toString()));
            introduce = new QStandardItem(tr("%1").arg(sql.value(2).toString()));

            group->setData(GROUP,CUSTOM_ROLE);
            master->setData(MASTER,CUSTOM_ROLE);
            introduce->setData(INTRODUCE,CUSTOM_ROLE);

            group_list.append(group);
            group_list.append(master);
            group_list.append(introduce);

            treeModel->appendRow(group_list);
            group_list.clear();
        }
    }else{
        qDebug()<<sql.lastError();
    }

    loadMember(sql);

    tree_view->setModel(treeModel);
}

void UserTree::loadAllUser()
{
    QSqlQuery sql(sysDB);
    treeModel->clear();
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("ID")<<QStringList("姓名"));
    QStandardItem * user;
    QStandardItem * introduce;
    QList<QStandardItem *> item_list;

    if(sql.exec("select id,introduce from user;")){
        while(sql.next()){
            user = new QStandardItem(tr("%1").arg(sql.value(0).toString()));
            introduce = new QStandardItem(tr("%1").arg(sql.value(1).toString()));
            user->setData(USER,CUSTOM_ROLE);
            item_list.append(user);
            item_list.append(introduce);
            treeModel->appendRow(item_list);
            item_list.clear();
        }
    }else{
        qDebug()<<sql.lastError();
    }
    tree_view->setModel(treeModel);
}
