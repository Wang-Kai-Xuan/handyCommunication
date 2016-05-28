* TOC
{:toc}

# 项目介绍
本软件是Qt5开发框架下开发的在局域网通讯的工具软件。

# 版本更新
采用Udp和Tcp协议

# 运行环境
由于Qt开发框架的跨平台特性，所有P主流平台都能够运行（window、Linux）

# 开发环境
Qt5+window

# 详细功能：
## 基本功能：
1. 广播
2. 组播
3. 私信
4. 发送文件

### root具有的权限：
1. 创建组:所需要的信息-组id、组长
2. 删除组
3. 修改组
4. 查看组

### 组长具有的权限：
1. 添加成员
2. 删除成员


## 扩展功能：
1. 向网内所有用户播放视频
2. 向组内用户播放视频
3. 向特定用户播放视频
4. 向网内所有用户播放音频
5. 向组内用户播放音频
6. 向特定用户播放音频


## 界面设计：
1. 本人信息及设置
2. 广播
3. 组播和私信
4. 视频播放窗口
5. 音频播放窗口

## 数据库设计：
数据表种类：
1. 注册用户信息（user）
2. 组信息（_group）
3. 用户聊天历史（user_history_id）
4. 组聊天历史(group_history_id)

```
table:
    user -> id password phone email qq sex introduce ip port isOnline
    _group -> id master_id introduce
    user_history_id -> id type content time
    group_history_id -> id sender content time
```
# 使用对象
任何在局域网内通讯的场合

# 通信协议
命令|数据
