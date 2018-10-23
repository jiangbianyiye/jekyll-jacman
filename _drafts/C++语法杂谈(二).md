C++语法杂谈(二)

### 别名

#### 普通类型别名

typedef和using都可用于基本类型、函数、类的别名，且二者不冲突，例如：

```c++
#include<iostream>
#include<map>

using namespace std;

class Test {
public:
	Test() {
		cout << "Test()" << endl;
   }
};

typedef Test classA;
using classA = Test;

void main()
{
    classA a; //Test()
	typedef unsigned int u_int;
	using u_int = unsigned int;

	typedef void(*funAlias)();
	using funAlias = void(*)();

	cout << typeid(u_int).name() << endl;	//unsigned int
	cout << typeid(funAlias).name() << endl;//void (__cdecl*)(void)
	return;
}
```

#### 命名空间别名

命名空间别名允许定义命名空间的另一个名称，常用作长的或嵌套过深的命名空间的简便使用方式。别名 alias_name 提供另一种访问 ns_name 的方法。alias_name 必须是先前未使用的名称。 alias_name 在引入它的作用域的期间内合法。 

 语法:

```c++
namespace alias_name = ns_name;	//(1)	
namespace alias_name = ::ns_name;	//(2)	
namespace alias_name = nested_name::ns_name;	//(3)	
```

例：

```c++
#include<iostream>
#include<map>

using namespace std;

namespace foo {
	namespace bar {
		namespace baz {
			int qux = 42;
		}
	}
}

namespace f = foo;
namespace fbz = f::bar::baz;

namespace mystd = std;

int main()
{
	mystd::cout << fbz::qux << '\n';//
	cin.get();
}
```

#### 模板别名

模板别名只能通过using关键字定义

```c++
#include<iostream>
#include<map>

using namespace std;

template<typename T>
using alias_map = std::map<std::string, T>;

template<typename T>
using alias_pair = std::pair<std::string, T>;

int main()
{
	alias_map<int> mapInt;
	mapInt.insert(alias_pair<int>("a",97));
	mapInt.insert(alias_pair<int>("b", 98));
	for (auto& i : mapInt) {
		cout << i.first.c_str() << "->"<<i.second << endl;
	}


	alias_map<string> mapString;
	mapString.insert(alias_pair<string>("c", "99"));
	mapString.insert(alias_pair<string>("d", "100"));
	for (auto& i : mapString) {
		cout << i.first.c_str() << "->" << i.second.c_str() << endl;
	}

	cin.get();
	return 0;
}
//输出：
//a->97
//b->98
//c->99
//d->100
```

### new/delete

 在使用new关键字创建一个类的对象时分三步：①分配足够的内存空间，②调用该类型的构造函数初始化对象，③返回对象指针。在使用delete关键字销毁一个对象时分两步：①调用对象的析构函数，②调用operator delete 释放内存空间。

new/delete即是关键字，又是运算符。在说它们是关键字时，由编译器对其编译为上述操作的相关代码。在说它们运算符时，他们是在创建\销毁对象时用到的运算，详细说明如下。

在创建和销毁对象过程中要用到如下操作符：

#### operator new 和operator delete

operator new 和 operator delete是 C++ 语言标准库的库函数 ，原型分别如下：

```c++
void *operator new(size_t);     //allocate an object
void *operator delete(void *);    //free an object

void *operator new[](size_t);     //allocate an array
void *operator delete[](void *);    //free an array
```

创建对象时，编译器向operator new传入要分配的内存空间大小，由其分配内存并返回相应地址指针，函数operator new 对应new关键字操作的①阶段。销毁对象时向operator传入对应内存空间首地址，由其完成内存释放的功能，函数operator delete对应delete操作②操作。new,delete在具体实现中一般借助C语言函数库的malloc和free实现。

加了`[]`的版本是为了解决动态数组内存分配的问题，下面解释加`[]`版的具体实现

看如下代码：（编译环境VS2015 32位）

```c++
#include <iostream>

using namespace std;

class A {
	int a;
public:
	A() {
		cout << "constructor A() called" << endl;
	}
	~A() {
		cout << "destructor ~A() called" << endl;
	}
	void show() {
		cout << a << endl;
	}

	void * operator new [](size_t size) {
		cout <<"operator new[] param size="<<size<< endl;
		//void * p = (void *)( (int)(malloc(size)) + 4); 
        //用上一句替换下一句将在运行时出错
		void *p = malloc(size);
		return p;
	}

	void operator delete [](void *p) {
		cout <<"operator delete[] param p="<< p << endl;
		free(p);
	}
};

int main() {
	A *a = new A();
	delete a;
	cout << endl;
	A *arr = new A[3];	
	cout << "objects array pointer point to "<<(void *)arr << endl;
	int * beforeArr4bit = (int *)((int)arr - 4);
	cout << "the int number in 4 bits brefor objects array is " << *beforeArr4bit << endl;
	delete[] arr;
	cin.get();
}
//输出：
//constructor A() called
//destructor ~A() called
//
//operator new[] param size = 16
//constructor A() called
//constructor A() called
//constructor A() called
//objects array pointer point to 029D72EC
//the int number in 4 bits brefor objects array is 3
//destructor ~A() called
//destructor ~A() called
//destructor ~A() called
//operator delete[] param p = 029D72E8
```

`A *arr = new A[3]`调用了3次构造函数，`delete[] arr`调用了3次析构函数，可以看出传入`operator new []`的参数大小要比3个类的总大小12大了4个字节，传入`operator delete []`的地址也要比实际指向的地址空间向上了4个字节。为什么？

这为了方便delete 运算符在释放连续的空间时决定要调用几次析构函数，构造空间时多出来的4个字节存储的便是这片空间存储的对象数目，因而在释放空间时，传递后的地址向前移动了四个字节。

这里还有一个问题，既然对象数组的指针指向的第一个对象在分配的内存空间起始位置向后偏移4个字节，为什么在operator new []中返回的地址是分配内存的起始地址。因为这一操作是编译器对关键字new操作的第③步来完成的。如果返回了偏移后的地址，反而会出错。



现在将上述类的析构函数注释掉，看看会发生什么

```c++
#include <iostream>

using namespace std;

class A {
	int a;
public:
	A() {
		cout << "constructor A() called" << endl;
	}
	//~A() {
	//	cout << "destructor ~A() called" << endl;
	//}
	void show() {
		cout << a << endl;
	}

	void * operator new [](size_t size) {
		cout << "operator new[] param size=" << size << endl;
		//void * p = (void *)( (int)(malloc(size)) + 4); 
		//用上一句替换下一句将在运行时出错
		void *p = malloc(size);
		return p;
	}

		void operator delete [](void *p) {
		cout << "operator delete[] param p=" << p << endl;
		free(p);
	}
};

int main() {
	A *a = new A();
	delete a;
	cout << endl;
	A *arr = new A[3];
	cout << "objects array pointer point to " << (void *)arr << endl;
	int * beforeArr4bit = (int *)((int)arr - 4);
	cout << "the int number in 4 bits brefor objects array is " << *beforeArr4bit << endl;
	delete[] arr;
	cin.get();
}
//输出：
//constructor A() called
//
//operator new[] param size = 12
//constructor A() called
//constructor A() called
//constructor A() called
//objects array pointer point to 033D1AA0
//the int number in 4 bits brefor objects array is - 33686019
//operator delete[] param p = 033D1AA0
```

什么？分配时没有多分4个字节，向operator delete [] 传递的指针也没有偏移？原因就是此时`operator delete []`不再调用析构函数，从而也就不用再保存对象的数目了，这一点在`new int[2]`也是一样的。

因此，new出来的对象数组如果没有自定义的析构函数的话，用delete是没有问题的，然而在有自定义析构函数的情况下，使用delete 而不是 delete []就会只调用一次析构函数，而导致可能的内存溢出。

因此new/delete；new [] /delete []要配套使用。

#### 在堆区、静态区分配对象

operator new 不仅仅是上面两种重载，new( 函数 ) 有六种重载形式

```c++
//throwing (1)	
void* operator new (std::size_t size);

//nothrow (2)	
void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;

//placement (3)	
void* operator new (std::size_t size, void* ptr) noexcept;
                           

//throwing (1)	
void* operator new[] (std::size_t size);

//nothrow (2)	
void* operator new[] (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;

//placement (3)	
void* operator new[] (std::size_t size, void* ptr) noexcept;
```

C++11默认调用形式(1)，即在分配内存失败时会抛出异常的版本。形式(2)在分配内存失败时返回nullptr而不会抛出异常。形式(3)不会分配内存，而是在参数ptr指向的内存地址上构造对象。

利用形式(3)可以在栈区，数据区分配对象。

```c++
#include<iostream>

using namespace std;

class B {
	
public:
	int a;
	B() {

	}
	B(int a) {
		this->a = a;
	}
	~B() {
		cout << "destructor ~A() called" << endl;
	}
	void show() {
		cout << a << endl;
	}

};

char * strRDDataSeg = "hello world!";

char strDataSeg[1024];

int main()
{
	char strStackSeg[1024];

	B * bInRDDataSeg = new(strRDDataSeg) B();
	//aRDDataSeg->a = 1; //异常，访问权限冲突，只读数据区不能写
	bInRDDataSeg->show();

	B * arrInDataSeg = new(strDataSeg) B[2]{2,3};
	for (int i = 0; i < 2; i++) {
		arrInDataSeg[i].show();
	}

	B * arrInStackSeg = new(strStackSeg) B[3]{B(4), B(5), B(6)};
	for (int i = 0; i < 3;i++) {
		arrInStackSeg[i].show();
	}

	cin.get();
	return 0;
}
//输出：
//1819043176
//2
//3
//4
//5
//6
```

参考：

[1] C++ Primer

\[2\] [operator new](http://www.cplusplus.com/reference/new/operator%20new/)

\[3\] [operator new []](http://www.cplusplus.com/reference/new/operator%20new[]/)

\[4\] [浅谈 C++ 中的 new/delete 和 new\[\]/delete\[\\] ](https://blog.csdn.net/hazir/article/details/21413833)

\[5\] [深入探究C++的new/delete操作符](https://kelvinh.github.io/blog/2014/04/19/research-on-operator-new-and-delete/)























# 待整理

void show(char ) = delete;//删除函数，精确匹配

函数声明 = delete

删除构造，析构，



__if_not_exits()

__if_exits()

可用于检测某个模板有没有根据某个类型实例化

`__if_exits(std::list<int>)`





int a = 0B1101;//0b111  0B0101代表二进制
	cout << a << endl;



conster常数表达式

```c++

//constexpr标志返回值或者其他表达式是常量
constexpr   int get()
{
	return 5;
}


void main()
{
	int a[5+get()];


	cin.get();
}
```

