---
layout: post
title: 通过NetBeans IDE连接虚拟机上的DB2数据库
tags: DB2
categories: DataBase
date: 2017-04-26 22:45:00 +0800
---
**原创博客，转载请注明出处**    
折腾了一下午虚拟机，成功的从宿主机用NetBeans IDE连接到了win xp虚拟机上的DB2数据库，做个小结。

### 测试从宿主机能否ping通虚拟机

这一步是为了测试宿主机能否与虚拟机沟通，不是必须。    
但是还是为了这个弄了半天，百度各种介绍VMware虚拟机下三种网络模式介绍，又是需要bridge模式又是要设在同一段ip地址下，又是要重置网络设置的。弄了半天都没ping通，谷歌了一下原来是因为防火墙。吐血...所以一定可以先**关闭虚拟机的防火墙**当然，如果对安全性有要求，不想关闭防火墙可以参考[这篇文章](http://www.sysprobs.com/enable-ping-in-xp)(但是为了个xp虚拟机好像没什么必要)。      
说了半天废话，进入正题。首先配置虚拟机网络，各种模式及其区别，参考[官方文档](https://pubs.vmware.com/workstation-12/index.jsp#com.vmware.ws.using.doc/GUID-6F7ECBA6-A29A-40AE-B565-3D0F2E74FEA4.html)。不推荐其他的是因为太乱太杂。这里我为了方便连接联网，直接用的net模式，自动获取ip地址。     
接下来在虚拟命令行输入`ipconfig`查看当前ip地址假设为192.168.233.128。接着在实机中开启命令行窗口输入`ping 192.168.233.128`  (ip改为你自己的)如果显示连通，请进入下一步。如果遇到问题，抱歉，可以尝试google。   

### 设置DB2通用驱动

首先，找到虚拟机中DB2的安装目录，到...\IBM\SQLLIB\java目录下，将文件db2jcc.jar与db2jcc_license_cu.jar文件拷出来，放到实机中。然后打开NetBeans按下图操作。    
![](/images/tgn0426_1.gif)

### 连接数据库

之后进行如下操作连接数据库

![](/images/tgn0426_2.gif)

这里连接到了在DB2中创建的SAMPLE示例数据库，请根据自己的需要修改。

### 查看数据库

![](/images/tgn0426_3.gif)

到此，我们的连接就成功结束了。   
连接服务器DB2数据库与此同理。