#include "user_tree.h"

void UserTree::newUI()
{
    tree_view = new QTreeView();
    treeModel = new QStandardItemModel;
    glay = new QGridLayout;
    child_glay = new QGridLayout;
    msg_input = new QTextEdit;
    msg_output = new QTextEdit;
    hide_btn = new QPushButton("隐藏(&H)");
    send_btn = new QPushButton("发送(&H)");
}

void UserTree::setUI()
{
    child_glay->addWidget(msg_output,0,0,1,1);
    child_glay->addWidget(msg_input,1,0,1,1);
    child_glay->addWidget(hide_btn,2,0,1,1);

    glay->addWidget(tree_view,0,0,1,1);
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
    msg_input->hide();
    msg_output->hide();
    hide_btn->hide();
    send_btn->hide();
}
void UserTree::onSendMessage()
{
//    QByteArray str = msg_input->document()->toPlainText().toUtf8();
//    udpSocket->sendBroadCast(str);
}

void UserTree::onRecvMessage()
{
    qDebug()<<"in UserTree"<<udpSocket->getNetWorkContent();
}

void UserTree::showMessage()
{
    msg_input->show();
    msg_output->show();
    hide_btn->show();
    send_btn->show();
}

void UserTree::onSelectUser(const QModelIndex &index)
{
    QString object;
    QVariant type = index.data(CUSTOM_ROLE);
    switch(type.toInt()){
    case ID:
        object = index.data().toString();
        qDebug()<<"ID object="<<object<<endl;
        break;
    case GROUP:
        showMessage();
        object = index.data().toString();
        qDebug()<<"GROUP object="<<object<<endl;
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
