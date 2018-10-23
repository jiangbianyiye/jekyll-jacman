C++智能指针

纯指针的使用会导致许多不可避免的错误，比如在分配空间和释放空间之间出现了异常、函数提前返回等，都将导致被分配的空间无法得到释放，因此，提出了智能指针的概念，自动完成内存的释放工作。

智能指针是一个`RAII`（`Resource Acquisition is initialization`）类模型 ，用来管理动态分配的内存。智能指针和普通指针的区别在于智能指针实际上是对普通指针加了一层封装，目的是为了使得智能指针可以方便的管理一个对象的生命期。 C++标准库提供的智能指针都在头文件memory中定义，有以下几种：

### auto_ptr

C++98引入，会在当前语句块结束后释放指针指向的空间。

使用示例：

```c++
//***************************************************************
class Test
{
public:
 Test(int a = 0 ) : m_a(a)
 {
 }
 ~Test( )
 {
  cout<<"Calling destructor"<<endl;
 }
public:
 int m_a;
};
//***************************************************************
void Fun( )
{
 int a = 0, b= 5, c;
 if( a ==0 )
 {
  throw "Invalid divisor";
 }
 c = b/a;
 return;
}
//***************************************************************
void main( )
{
 try
 {
  std::auto_ptr<Test> p( new Test(5) ); 
  Fun( );
  cout<<p->m_a<<endl;
 }
 catch(...)
 {
  cout<<"Something has gone wrong"<<endl;
 }
}
```

上例中，即使发生异常，动态分配的空间也能正常释放，这是因为在引发异常时发生堆栈展开(stack unwinding )。由于属于try块的所有本地对象都被销毁，`p` 超出了该作用域 ,并释放关联的内存。

auto_ptr的设计有其缺陷，查看下例：

```c++
#include<iostream>
#include<memory>
using namespace std;
 
class A
{
public:
    void show() {  cout << "A::show()" << endl; }
};
 
int main()
{
    // p1 is an auto_ptr of type A
    auto_ptr<A> p1(new A);
    p1 -> show(); //A::show()
 
    // returns the memory address of p1
    cout << p1.get() << endl; //0x1086c20
 
    // copy constructor called, this makes p1 empty.
    auto_ptr <A> p2(p1);
    p2 -> show(); //A::show()
 
    // p1 is empty now
    cout << p1.get() << endl; // 0
 
    // p1 gets copied in p2
    cout<< p2.get() << endl; //0x1086c20
 
    return 0;
}
```

在上例中，展示了auto_ptr的独占性，即同一资源只能被一个auto_ptr指向，当使用拷贝构造或赋值操作时，会传递保存的指针，而不是通过赋值的方式，前一个auto_ptr对象的指向在传递完成后将被清空。这就导致了当其auto_ptr作为参数传递到其他函数时，会导致函数返回后空间被释放，可能引发运行时错误。由于在释放空间时调用的时delete，它不能封装new []产生的指针。此外，由于它不能复制的特性，它还不能在标准容器中使用，因此在C++11中被废弃，C++11提出了一组新的智能指针。

### unique_ptr

unique_ptr的提出是为了取代auto_ptr，它也基于独占模型，但是改良了安全性。不再是虚假的拷贝（传递而不是拷底层指针，并将原来的指针清零），而是阻止直接的拷贝，允许使用std::move转移所有权。并增加了对数组的支持。

如：

```c++
class A
{
public:
	void show()
	{
		std::cout << "A::show()" << std::endl;
	}
};
int main()
{
	std::unique_ptr<A> p1(new A);
	p1->show();

	// returns the memory address of p1
	std::cout << p1.get() << std::endl;

	//transfers ownership to p2
	std::unique_ptr<A> p2 = move(p1);
	p2->show();
	std::cout << p1.get() << std::endl;
	std::cout << p2.get() << std::endl;

	//transfers ownership to p3
	std::unique_ptr<A> p3 = std::move(p2);
	p3->show();
	std::cout << p1.get() << std::endl;
	std::cout << p2.get() << std::endl;
	std::cout << p3.get() << std::endl;

	std::cin.get();
	return 0;
}
```

输出：

```
A::show()
02E03BE0
A::show()
00000000
02E03BE0
A::show()
00000000
00000000
02E03BE0
```

make_unique宏表示生成unique_ptr智能指针，用法如下

```C++
#include<iostream>
#include<memory>

class A
{
public:
	void show()
	{
		std::cout << "A::show()" << std::endl;
	}
};

int main()
{
	auto uptr = std::make_unique<int[]>(10);
	uptr[5] = 17;
	std::cout << uptr[5] << std::endl;//17

	auto uptr2 = std::make_unique<A>();
	uptr2->show();//A::show()

	std::cin.get();
	return 0;
}
```

### shared_ptr

共享指针，使用引用计数所有权模型(reference counting ownership model)。多个共享指针能够指向同一个对象，当所有指针都超出其作用域时，内存会被自动回收。shared_ptr是线程安全的

示例：

```c++
#include<iostream>
#include<memory>

class A
{
public:
	void show()
	{
		std::cout << "A::show()" << std::endl;
	}
};

int main()
{
	std::shared_ptr<A> p1(new A);
	std::cout << p1.get() << std::endl;
	p1->show();
	std::shared_ptr<A> p2(p1);
	std::cout << p1.get() << std::endl;
	std::cout << p2.get() << std::endl;

	// Returns the number of shared_ptr objects 
	//referring to the same managed object.
	std::cout << p1.use_count() << std::endl;
	std::cout << p2.use_count() << std::endl;

	// Relinquishes ownership of p1 on the object
	//and pointer becomes NULL
	p1.reset();
	std::cout << p1.get() << std::endl;
	std::cout << p2.get() << std::endl;
	std::cout << p1.use_count() << std::endl;
	std::cout << p2.use_count() << std::endl;
	
	std::cin.get();
	return 0;
}
```

输出：

```
00BFE448
A::show()
00BFE448
00BFE448
2
2
00000000
00BFE448
0
1
```

使用误区：同一个内存对象不能分配给多组shared_ptr，不能使用shared_ptr进行循环引用，否则会造成内存溢出，详细内容参看[C++11 Smart Pointers](https://www.codeproject.com/Articles/541067/Cplusplus-Smart-Pointers)

### weak_ptr

weak_ptr只能从shared_ptr创建，它的出现是为了解决环状引用导致的引用计数无法减到0而造成内存溢出的问题(两篇参考中均有详细解释，在此略过)。弱指针引用指针，但不拥有指针，因此不能进行*、->操作

使用示例：

```c++
#include <iostream>
#include <memory>

using namespace std;

class B;
class A
{
public:
 A(  ) : m_a(5)  { };
 ~A( )
 {
  cout<<" A is destroyed"<<endl;
 }
 void PrintSpB( );
 weak_ptr<B> m_sptrB;
 int m_a;
};
class B
{
public:
 B(  ) : m_b(10) { };
 ~B( )
 {
  cout<<" B is destroyed"<<endl;
 }
 weak_ptr<A> m_sptrA;
 int m_b;
};

void A::PrintSpB( )
{
 if( !m_sptrB.expired() )
 {  
  cout<< m_sptrB.lock( )->m_b<<endl;
 }
}

int main( )
{
 shared_ptr<B> sptrB( new B );
 shared_ptr<A> sptrA( new A );
 sptrB->m_sptrA = sptrA;
 sptrA->m_sptrB = sptrB;
 sptrA->PrintSpB( ); 
 return 0;
}
```

输出：

```
10
 A is destroyed
 B is destroyed
```

参考：

[C++11 Smart Pointers](https://www.codeproject.com/Articles/541067/Cplusplus-Smart-Pointers)

[auto_ptr, unique_ptr, shared_ptr and weak_ptr](https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2/)