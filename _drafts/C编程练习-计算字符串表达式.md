---
layout: post

title: C编程练习-字符串操作高级

categories: C

tags: C exercise
---

删除字串，替换字串，计算字符串表达式(四则运算及小括号标识优先级)

### 删除字符串中某一字符

使用双指针p1、p2，每次将p2指向的内容赋值给p1，遇到要删除的字符时，将p2后移，循环进行的字符串尾部即可完成删除字符串中某一特定字符

```c
void delchr(char * str, char ch) {
	char *p1 = str;
	char* p2 = str;
	do {
		if (*p2 == ch) {
			p2++;
		}
	} while ((*p1++ = *p2++) != '\0');
}
```

### 删除字符串中某一特定字串

同样利用双指针的思想,找到匹配的字符串后，将p2移到母串中匹配到的字串后的地址

```c
void delsubstr(char *str, const char *sub) {
	char *p1 = str;
	char* p2 = str;
	do{
		char *subp = sub, *strp = p2;
		while (*subp != '\0'&&*subp++ == *strp++);
		if (*subp == '\0') {	 //找到字串
			p2 = strp;
		}
	} while ((*p1++ = *p2++) != '\0');
}
```

### 将字符串中某一字串替换为指定字符串

注：被替换的字串与要拿来替换字串的长度不一定相同

```c

```

