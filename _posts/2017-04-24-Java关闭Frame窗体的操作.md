---
layout: post
title: Java关闭Frame窗体的操作
tags: Java
categories: Java
date: 2017-04-24 20:03:20 +0800
---

这篇博客主要讨论Java Frame类与JFrame的差别，以及如何关闭Frame窗体。

### Java中JFrame与Frame的区别

区别主要有以下几点：
+ Frame是java.awt包中的类，自从JDK1.0就存在了；JFrame是javax.swing包中的类，在JDK1.1.3后才出现。
+ Frame继承Window类，JFrame继承Frame类。
+ JFrame支持关闭操作的设定，即能直接调用setDefaultCloseOperation()函数

再来说说Java中两个与图形界面有关的包AWP与Swing

AWP:
- 被称作Heavy Weight组件
- 与平台相关
- 使用操作系统的GUI组件
Swing:
- 被称作Light Weight组件
- 与平台无关
- 使用纯Java组件

Java的GUI相关类的继承关系：

```
     Object
        |
    Component
 -------|--------
 |              |
JComponent     Window
 |              |
JPanel         Frame
                |
               JFrame
```
现在Java GUI开发者大都使用Swing,因为它有很多高级特性，并且支持LookAndFell（所见即所得）特性。有兴趣的话可以在[http://docs.oracle.com/javase/tutorial/uiswing/lookandfeel/](http://docs.oracle.com/javase/tutorial/uiswing/lookandfeel/)中查看。

### 设置关闭Frame窗口时的选项

设置关闭JFrame窗口时的行为比较简单,调用[setDefaultCloseOperation()](https://docs.oracle.com/javase/7/docs/api/javax/swing/JFrame.html#setDefaultCloseOperation(int))即可。    
用法示例：

```java
public class MyFrame extends JFrame {
    public MyFrame() {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    
    ...
}
```

然而Frame没有该函数，要设置关闭Frame窗口时的行为怎么办呢。    
要知道JFrame以及Frame都是java.awt.Window的子类。可以使用Windows中的方法。   
使用[java.awt.Window.addWindowListener(java.awt.event.WindowListener l)](https://docs.oracle.com/javase/7/docs/api/index.html?javax/swing/JFrame.html)例如：
```java
public class MyFrame extends JFrame {
    public MyFrame() {
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent e) {
                System.exit(0);
            }
    });
} 
...
}
```
