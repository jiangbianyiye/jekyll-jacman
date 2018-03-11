---
layout: post

title: C编程小练习-函数

categories: C

tags: C exercise

---

#### 可变参数函数

##### 用参数标记输入个数

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>//标准参数


int add(int num,...)//...代表可变参数,num代表参数个数
{
	int res = 0;//结果
	va_list  argp;//存储参数开始的地址
	va_start(argp, num);//从首地址开始，读取num后面的数据
	for (int i = 0; i < num;i++)
	{

		res += va_arg(argp, int);//读取一个数据按照int解析
	}
	va_end(argp);//结束读取
	return res;

}

int main()
{
    printf("%d\n", add(3, 1, 2, 3));
	printf("%d\n", add(4, 1, 2, 3,4));
	printf("%d\n", add(5, 1, 2, 3, 4,5));   
    return 0;
}
```

##### 用标记符标记结束

```c
void add(int start,...)
{
    int res=0;
	va_list  argp;//存储参数开始的地址
	va_start(argp, start);//从首地址开始，读取num后面的数据
	int argvalue = start;//第一步初始化
	do 
	{
        res+=argvalue;
		argvalue =va_arg(argp, int);//不断读取
	} while (argvalue != -1);//读取到-1表示结束
	va_end(argp);//结束读取
	return res;
}
```

[参考：可变参数详解](https://www.cnblogs.com/clover-toeic/p/3736748.html)

##### 函数指针

