1 项目名称：简讯

功能概要：能够进行局域网内的通讯聊天工具

详细功能：
    基本功能：
        1.广播
        2.组播
        3.私信
        4.发送文件

    扩展功能：
        1.向网内所有用户播放视频
        2.向组内用户播放视频
        3.向特定用户播放视频
        4.向网内所有用户播放音频
        5.向组内用户播放音频
        6.向特定用户播放音频


2 界面设计：
    显示用户、组
    显示消息框
    消息输入框
    视频播放窗口
    音频播放窗口

3 数据库设计：
    数据表：注册用户信息（user）、组信息（group）、用户聊天历史（user_history_id）、组聊天历史(group_history_id)

    table:
        user -> id password phone email qq sex introduce ip port isOnline
        group -> id master_id introduce
        user_history_id -> id type content time
        group_history_id -> id type content time

4 使用对象
    ...
