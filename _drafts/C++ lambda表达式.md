C++ 函数闭包

维基百科对[闭包](https://zh.wikipedia.org/wiki/%E9%97%AD%E5%8C%85_(%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6))的定义如下：

在[计算机科学](https://zh.wikipedia.org/wiki/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6)中，**闭包**（英语：Closure），又称**词法闭包**（Lexical Closure）或**函数闭包**（function closures），是引用了自由变量的函数。这个被引用的自由变量将和这个函数一同存在，即使已经离开了创造它的环境也不例外。所以，有另一种说法认为闭包是由函数和与其相关的引用环境组合而成的实体。闭包在运行时可以有多个实例，不同的引用环境和相同的函数组合可以产生不同的实例。

通俗的说，函数 带上状态, 就成为闭包。  带上状态是指这个闭包有属于自己的变量, 这些个变量的值是创建闭包的时候设置的, 并在调用闭包的时候, 可以访问这些变量。

### 函数对象/伪函数

Function object或者说Functor在C++中就表现为就表现为重载了operator()的对象。函数对象的出现，可以让函数能保有自己的状态，而不必依靠全局变量。

查看下面一个例子：

```c++
#include<algorithm>
#include <iostream>

// A Functor
class increment
{
private:
	int num;
public:
	increment(int n) : num(n) {  }

	// This operator overloading enables calling
	// operator function () on objects of increment
	int operator () (int arr_num) const {
		return num + arr_num;
	}
};

// Driver code
int main()
{  
	int a = 10;
	int b = increment(1)(a);//等价于int b = increment(1).operator()(a);
	std::cout << b << std::endl;//11
    
	int arr[] = { 1, 2, 3, 4, 5 };
	int n = sizeof(arr) / sizeof(arr[0]);

	std::transform(arr, arr + n, arr, increment(5));
	for (int i = 0; i<n; i++)
		std::cout << arr[i] << " ";	
	std::cout << std::endl;// 6 7 8 9 10

	std::transform(arr, arr + n, arr, increment(-2));
	for (int i = 0; i<n; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;//4 5 6 7 8

	std::cin.get();
}
```

参考：

[Functors in C++](https://www.geeksforgeeks.org/functors-in-cpp/)

[C++ Functors - and their uses](https://stackoverflow.com/questions/356950/c-functors-and-their-uses)

[std::transform](http://en.cppreference.com/w/cpp/algorithm/transform)

### lambda表达式

lambda表达式本质就是可以捕获变量的匿名函数对象。

查看以下示例：

```c++
#include <iostream>
#include <algorithm>
using namespace std;

template<typename F>
void show(F & f) {

	cout << typeid(f).name() << endl;
	double a = f(1);
	cout << (double)f(3.5) << endl;
}

int main()
{
	double db = 1;
	
	auto adddb = [db](double a) {
		return db + a;
	};
	cout << typeid(adddb).name() << endl;
    //class <lambda_2a0cc637855c0b10cd1814336bb3f7bf>
	cout << sizeof(adddb) << endl;//8
	cout << adddb(2.5) << endl;	//3.5
	show(adddb);
	//class <lambda_2a0cc637855c0b10cd1814336bb3f7bf>
	//4.5
	cin.get();
	return 0;
}	
```

其中

```c++
auto adddb = [db](double a) {
		return db + a;
};
```

可以看作被翻译成

```c++
class SomeAnonymousType{
private:
    double db;
public:
    SomeAnoymousType(double init_db):db(init_db){}
    
    double operator()(double a) const{
        return db+a;
    }
}；

auto adddb = SomeAnonymousType(db);
```

lambda表达式使对策略模式的一种极佳实践，极大的方便了对新的操作的定义，一个使用示例如：

```c++
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	float x[]{ 1.0,-2.0,-1.5,1.6,2.3 };
	
	auto comp =   [](float a, float b) {
			return (std::abs(a) < std::abs(b));
	}; 

	std::sort(x, x + 5,comp );

	for (auto i : x) {
		cout << i<<endl;
	}//1 -1.5 1.6 -2 2.3

	cin.get();
	return 0;
}

```

 当 Lambda 表达式的捕获列表为空时，Lambda 表达式还能够作为一个函数指针进行传递

```c++
#include <iostream>

using foo = void(int);  // 定义函数指针别名
void functional(foo f) {
    f(1);
}

int main() {
    auto f = [](int value) {
        std::cout << value << std::endl;
    };
    functional(f);  // 函数指针调用
    f(1);           // lambda 表达式调用
    return 0;
}
```

lambda表达式详细语法如下

`[ captures ] ( params ) mutable constexpr(c++17) exception attribute -> ret { body } `

- captures: 捕获列表，将在后面详细叙述
- params: 函数参数列表，可以省略（但是后面必须紧跟函数体）； 
- mutable: 可选，表明可以修改以值传递方式捕获的变量，但修改的是副本
- constexpr:可选，表示lambda表达式可以被看作一个常量
- exception: 可选，指明lambda表达式可以抛出的异常
- attribute:可选，指明lambda表达式的特性
- ret: 可选，函数返回值
- body: 函数体

lambda表达式的捕获方式可以分为值捕获和引用捕获，使用示例如下：

```
[]：不捕获；
[=]：以值捕获所有变量；
[&]：以引用捕获所有变量；
[x]：仅以值捕获x，其它变量不捕获；
[&x]：仅以引用捕获x，其它变量不捕获；
[=, &x]：值捕获所有变量，但x例外，通过引用捕获；
[&, x]：引用捕获所有变量，但x例外，通过值捕获；
[this]：通过引用捕获当前对象（实际是复制指针）；
[*this]：通过传值方式捕获当前对象；

有些变量即使不指明捕获也能使用：is a non-local variable or has static or thread local storage duration (in which case the variable cannot be captured), or is a reference that has been initialized with a constant expression.
有些变量不指明捕获仅能取值：has const non-volatile integral or enumeration type and has been initialized with a constant expression, or is constexpr and trivially copy constructible.
```

参考：

[Lambda expressions (since C++11)](http://en.cppreference.com/w/cpp/language/lambda)

[C++ lambda表达式与函数对象](https://blog.csdn.net/xiaohu2022/article/details/70183436)

[C++11 Lambda函数闭包的类型与存储形式](http://hczhcz.github.io/2014/11/04/type-and-storage-of-lambda.html)

### 函数适配器

绑定器`std::bind`可以将函数对象的参数绑定至特定的值。对于没有绑定的参数可以使用`std::placeholers::_1, std::placeholers::_2`等标记 

使用示例：

```c++
#include<random>
#include<iostream>
#include<memory>
#include<functional>

void f(int n1, int n2, int n3, const int& n4, int n5)
{
	std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << std::endl;
}

int g(int n1)
{
	return n1;
}

struct Foo
{
	void print_sum(int n1, int n2)
	{
		std::cout << n1 + n2 << std::endl;
	}
	int data = 10;
};

int main()
{
	using namespace std::placeholders; //for _1, _2, _3...

	// demonstrates argument reordering and pass-by-reference
	int n = 7;
	// (_1 and _2 are from std::placeholders, and 
	// represent future arguments that will be passed to f1)
	auto f1 = std::bind(f, _2, _1, 42, std::cref(n), n);
	n = 10;
	f1(1, 2, 1001);// 1 is bound by _1, 2 is bound by _2, 1001 is unused
				   //makes a call to f(2, 1, 42, n, 7)  

	// nested bind subexpressions share the placeholders
	auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
	f2(10, 11, 12);// makes a call to f(12, g(12), 12, 4, 5);

	// common use case: binding a RNG with a distribution
	std::default_random_engine e;
	std::uniform_int_distribution<> d(0, 10);
	auto rnd = std::bind(d, e);	// a copy of e is stored in rnd
	for (int n = 0; n<10; ++n)
		std::cout << rnd() << ' ';
	std::cout << std::endl;

	// bind to a pointer to member function
	Foo foo;
	auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
	f3(5);

	// bind to a pointer to data member
	auto f4 = std::bind(&Foo::data, _1);
	std::cout << f4(foo) << std::endl;

	// smart pointers can be used to call members of the referenced objects, too 
	std::cout << f4(std::make_shared<Foo>(foo)) << std::endl;
	std::cout << f4(std::make_unique<Foo>(foo)) << std::endl;

	std::cin.get();
	return  0;
}
```

输出：

```
2 1 42 10 7
12 12 12 4 5
6 3 10 9 4 7 0 6 5 4
100
10
10
10
```

参考：

[Bind function and placeholders in C++](https://www.geeksforgeeks.org/bind-function-placeholders-c/)

[std::bind](http://en.cppreference.com/w/cpp/utility/functional/bind)

### 函数包装器

类模板std :: function是一个通用的多态函数包装器。 std :: function的实例可以存储，复制和调用任何可调用类型，如lambda表达式，绑定表达式或其他函数对象，以及指向成员函数的指针和指向数据成员的指针。

使用示例：

```c++
#include<functional>
#include<iostream>

using namespace std;

struct Foo {
	Foo(int num) :num(num) {}
	void print_add(int i) const { cout << num + i << endl; }
	int num;
};

void print_num(int i)
{
	cout << i << endl;
}

struct PrintNum {
	void operator()(int i)const
	{
		cout << i << endl;
	}
};

int main()
{
	// store a free function
	function<void(int)>	 f_display = print_num;
	f_display(19);  //19

	//store a call to a function object
	function<void(int)> f_display_obj = PrintNum();
	f_display_obj(8);  //8

	//store a lambda
	function<void()> f_display_36 = []() {print_num(36); };
	f_display_36();	 //36

	//store the result of a call to std::bind
	function<void()> f_display_1313 = std::bind(print_num, 1313);
	f_display_1313();  //1313

	//store a call to member function
	function<void(const Foo&, int)>f_add_display = &Foo::print_add;
	const Foo foo(1654);
	f_add_display(foo,15);	//1669
	f_add_display(15, 6);	//21

	//store a call to a member function and object
	using std::placeholders::_1;
	function<void(int)> f_add_dispaly2 = std::bind(&Foo::print_add, foo, _1);
	f_add_dispaly2(2);	//1656

	//store a call to a member function and object ptr
	function<void(int)> f_add_dispaly3 = std::bind(&Foo::print_add, &foo, _1);
	f_add_dispaly3(3);	//1657

	//store a call to a data member accessor
	function<int(Foo const&)> f_num = &Foo::num;
	cout << "num: " << f_num(foo) << endl; //num: 1654

	cin.get();
	return 0;
}
```

参考:

[std :: function](http://en.cppreference.com/w/cpp/utility/functional/function)

