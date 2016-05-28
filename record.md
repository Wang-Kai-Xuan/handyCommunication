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

