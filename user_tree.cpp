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
    send_btn = new QPushButton("发送(&H)");
    chat_type = new QLabel("...");
    chat_id= new QLabel("...");
    view_type = new QComboBox;
}

void UserTree::setUI()
{
    view_type->addItem("用户树");
    view_type->addItem("所有用户");
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
}

void UserTree::setConnect()
{
    connect(tree_view,SIGNAL(entered(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(activated(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(tree_view,SIGNAL(clicked(QModelIndex)),this,SLOT(onSelectUser(QModelIndex)));
    connect(hide_btn,SIGNAL(clicked(bool)),this,SLOT(onHideMessage()));
    connect(send_btn,SIGNAL(clicked(bool)),this,SLOT(onSendMessage()));
    connect(udpSocket,SIGNAL(readFinished()),this,SLOT(onRecvMessage()));
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
    sercetChat(id);
}

void UserTree::onRecvMessage()
{
    QStringList list =  udpSocket->getNetWorkContent().split(SEPARATE);
    if(list.size() != 3) return ;
    if(list.at(0) != QString(SECRET_CHAT)) return ;
    msg_output->append(QString("<私信:%1>%2").arg(list.at(1)).arg(list.at(2)));
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

void UserTree::groupChat(QString & group_id)
{

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
    udpSocket->send(QString(SECRET_CHAT)+QString(SEPARATE+user_id+SEPARATE+msg_input->document()->toPlainText()),getUserIP(user_id));
    msg_input->clear();
}

void UserTree::onSelectUser(const QModelIndex &index)
{
    QString object;
    QVariant type = index.data(CUSTOM_ROLE);
    switch(type.toInt()){
    case ID:
        object = index.data().toString();
        showMessageUI();
        chat_type->setText("私信->");
        chat_id->setText(object);
        break;
    case GROUP:
        object = index.data().toString();
        showMessageUI();
        chat_type->setText("组播->");
        chat_id->setText(object);
        break;
    }
}

UserTree::UserTree(QSqlDatabase &db, Udp *socket):sysDB(db)
{
    udpSocket = socket;
    newUI();
    setUI();
    loadGroup();
    setConnect();
    onHideMessage();
}

void UserTree::loadGroup()
{
    QSqlQuery sql(sysDB);
    group_list.clear();
    treeModel->clear();
    QStandardItem * group;
    treeModel->setHorizontalHeaderLabels(QStringList()<<QStringList("编号")<<QStringList("组长")<<QStringList("说明"));
    if(sql.exec("select id master_id from _group;")){
        while(sql.next()){
            group = new QStandardItem(tr("%1").arg(sql.value(0).toString()));
            group->setData(GROUP,CUSTOM_ROLE);
            treeModel->appendRow(group);
            group_list.clear();
        }
    }else{
        qDebug()<<sql.lastError();
    }

    QStandardItem * item;
    QStandardItem * item_id;
    QStandardItem * item_name;
    QStandardItem * item_status;
    QList<QStandardItem *> member_list;
    for(int count = 0; count < treeModel->rowCount();count++){
        item = treeModel->item(count);
        if(sql.exec(QString("select member_id from _group_member_%1;").arg(treeModel->index(count,0).data().toString()))){
            while(sql.next()){
                item_id = new QStandardItem(QString("%1").arg(sql.value(0).toString()));
//                item_name = new QStandardItem(QString("%1").arg(sql.value(1).toString()));
//                item_status = new QStandardItem(QString("%1").arg(sql.value(2).toString()));

                item_id->setData(ID,CUSTOM_ROLE);
//                item_name->setData(NAME,CUSTOM_ROLE);
//                item_status->setData(STATUS,CUSTOM_ROLE);

                member_list.append(item_id);
//                member_list.append(item_name);
//                member_list.append(item_status);
                item->appendRow(member_list);
                member_list.clear();
            }
        }
    }
    tree_view->setModel(treeModel);
}
