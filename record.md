# 错误和警告
1. QLayout: Attempting to add QLayout "" to AudioPlayer "", which already has a layout
    QMainWindow是默认有layout的，所以再次设置layout会失效。
2. Q_ASSERT有什么用？

# 知识点
1. explicit关键字有什么用？explicit构造函数只能被显式调用
2. 可以在关闭应用程序时如何执行一些操作：

```
重写函数：void QWidget::closeEvent(QCloseEvent * event)
this event handler is called with the given event when Qt receives a window close request for a top-level widget from the window system.
```

# 如何实现在两个SQL查询添加QTreeView数据
4. QSqlQuery提供执行和操作的SQL语句的方法
5. QSqlQuery嵌套两个SQL查询，将得到的数据处理后添加到QTreeView中
6. 示例代码

```C++
void UserTree::loadMember(QSqlQuery &sql)
{
    QStandardItem * group;
    QStandardItem * member_id;
    QList<QStandardItem *>  item_list;
    QString introduce;
    QSqlQuery &sql1 = sql;
    for(int count = 0; count < treeModel->rowCount();count++){
        group = treeModel->item(count);
        if(sql.exec(QString("select member_id from _group_member_%1;").arg(treeModel->index(count,0).data().toString()))){
            while(sql.next()){
                member_id = new QStandardItem(QString("%1").arg(sql.value(0).toString()));
                member_id->setData(ID,CUSTOM_ROLE);
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
```

# 获取用户的输入数据
QInputDialog可以用来获取一行输入的数据