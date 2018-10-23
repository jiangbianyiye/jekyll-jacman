C++语法杂谈(一)

### 类型严格

C语言可以在不同类型的指针之间赋值时，而C++在不同类型指针之间赋值时会编译不通过，如下面的程序片段：

```c++
int main()
{
	int a = 0;
	long* p = &a; //error C2440 : “初始化”: 无法从“int *”转换为“long *”
	return 0;
}
```

#### nullptr

```c++
#include<iostream>

using namespace std;

void show(int num)
{
	cout << "int" << endl;
}
void show(int *p)
{
	cout << "int *" << endl;
}

void main()
{
	//NULL C风格的空指针，被视为整数
	//nullptr C++ 风格的空指针
	cout << typeid(NULL).name() << endl; //int 
	cout << typeid(nullptr).name() << endl;	//std::nullptr_t
	show(NULL);	 //int 
	show(nullptr);	//int *
	cin.get();
}
```

#### constexpr

C++11 提供了 `constexpr` 让用户显式的声明函数或对象构造函数在编译时会成为常数。 

例如：

```c++
constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}
```

此外，`constexpr` 的函数可以使用递归： 

```c++
constexpr int fibonacci(const int n) {
    return n == 1 || n == 2 ? 1 : fibonacci(n-1)+fibonacci(n-2);
}
```

C++14开始，onstexptr 函数可以在内部使用局部变量、循环和分支等简单语句

 ```c++
//C++11 标准下无法通过
constexpr int fibonacci(const int n) {
    if(n == 1) return 1;
    if(n == 2) return 1;
    return fibonacci(n-1)+fibonacci(n-2);
}
 ```

#### 类型转换

C风格的类型转换为TYPE b = (TYPE)a。 

C++风格的类型转换有4种：

1. `static_cast <type-id> ( expression )`

   常用于转换数值数据类型，不执行运行时类型检查,无法转换掉 const、volatile 或 __unaligned 特性

2. `dynamic_cast < type-id > ( expression )`

   type-id必须是类的指针、类的引用或者void *；如果type-id是类指针类型，那么expression 必须是一个指针，如果type-id是一个引用，那么expression 须是左值。dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。在类层次间进行上行转换时，dynamic_cast和static_cast 的效果是一样的；在进行下行转换时，由于dynamic_cast具有类型检查的功能，比static_cast 更安全。

3. `reinterpret_cast < type-id > ( expression )`,去掉类型的const或voliatile属性

   将任何指针转换为任何其他指针类型。 也允许将任何整数类型转换为任何指针类型以及反向转换,不能丢掉 const、volatile 或 __unaligned 特性

4. `const_cast < type-id > ( expression )`

   移除expression的 const、volatile 和 \_\_unaligned 特性，。除了const 、volatile、\_\_unaligned修饰之外， type_id和expression 的类型是一样的。

总结：

- 类指针或引用的上行转换static_cast 和 dynamic_cast 都可以
- 类指针或引用的下行转换用dynamic_cast并且判断转换后是否为空
- 基本数据类型之间的转换用static_cast, 但是由于数值范围的不同，需要用户保证转换的安全性
- 不同类型之间的指针或引用的转换用reinterpret_cast，它的本质是对指向内存的比特位的重解释
- 消除数据的const、volatile、__unaligned属性，用const_cast

### 类型推导

#### auto

在C语言和C++98 中auto用于声明变量为自动变量，意为变量拥有自动的生命周期。

C++11 开始auto可以在声明变量的时候根据变量初始值的类型自动为此变量选择匹配的类型。auto的自动类型推断发生在编译期，所以使用auto并不会造成程序运行时效率的降低。

C++14返回值设为auto时，能自动推理类型

C++11返回值设为auto时，需要用->指定类型

```c++
#include<iostream>

using namespace std;

//C++14中不需要用decltype指名返回值类型
template<typename T1,typename T2>
auto add(T1 a, T2 b)->decltype(a+b)
{
	return a + b;
}


//int(* fun1() )()
auto fun1()->int(*)() {
	return nullptr;
}



//int(*  (*  fun2(void)  )(int)  )(int,int)
auto fun2(void)->auto(*)(int x)->int(*)(int a, int b)
{
	return nullptr;
}

//int(*  (*  fun3(int,int(*)(int,int) )  )(int(*)(int,int))  )(int,int(*)(int, int(*)(int, int)))
auto fun3(int a, int(*)(int, int))->auto(*)(int(*)(int, int))
	-> int(*)(int, int(*)(int, int(*)(int, int)))
{
	return nullptr;
}

void main()
{
	auto str("hello world");
	auto boo(1 && 0 && 3 || -1);//仅在初始化时可以用() 代替=
	cout << typeid(str).name() << " " << str << endl;//char const * hello world
	cout << typeid(boo).name() << " " << boo << endl;//bool 1
	auto booC(boo);	
	cout << typeid(booC).name() << " " << booC << endl;	//bool 1
	cout << typeid(add(1,2)).name() << " " << add(1, 2) << endl;	//int 3
	cout << typeid(add(1, 2.0)).name() << " " << add(1, 2.0) << endl;	//double 3

	cout << typeid(fun1).name() << endl;
	//int (__cdecl*__cdecl(void))(void)
	cout << typeid(fun2).name() << endl;
	//int (__cdecl*(__cdecl*__cdecl(void))(int))(int,int)
	cout << typeid(fun3).name() << endl;
	//int (__cdecl*(__cdecl*__cdecl(int,int (__cdecl*)(int,int)))(int (__cdecl*)(int,int)))(int,int (__cdecl*)(int,int (__cdecl*)(int,int)))

	cin.get();
}
```

#### decltype

decltype关键字和auto相互对应的，它们经常在一些场所配合使用。decltype可以在编译的时候判断出一个变量或者表达式的类型，

```c++
#include<iostream>

using namespace std;

template<class T>
void show(T& p)
{
	decltype(p) num(p);
	cout << num << endl;
}


void main()
{

	double a = 10.0;
	decltype(a) b = 5.0;
	cout << b << endl; //5
	show(a); //10
	cin.get();

}
```

#### auto和decltype的区别

TO-DO



### 统一的初始化风格

在传统 C++ 中，不同的对象有着不同的初始化方法，例如普通数组、POD （plain old data，没有构造、析构和虚函数的类或结构体）类型都可以使用 `{}` 进行初始化，也就是我们所说的初始化列表。而对于类对象的初始化，要么需要通过拷贝构造、要么就需要使用 `()` 进行。这些不同方法都针对各自对象，不能通用。 

C++11把初始化列表的概念绑定到了类型上，并将其称之为 `std::initializer_list` ，统一了初始化的写法，任何数据类型，都可以用花括号来初始化，例如

```c++
//下面三行是C++03的风格
int a1=1;
int a2=int(1);
int a3(1);
//下面是C++11新增的初始化方式：
int a4={1};
int a5{1};
```

更多示例：

```c++
char str[10]{ "calc" };
wchar_t *wp(L"ABCDEFG");
int a[5]{ 1,2,3,4,5 };
```

用初始化列表还能省略返回值类型，例如

```c++
A func()
{
    //C++03风格：
    return A(123,456);
    //C++11风格：
    return {123,456};
}
```

当初始化列表用于内置类型的变量时，如果存在丢失信息的风险，则编译器将报错，提示需要进行收缩转换。

 ```c++
int main()
{
	int a = 23;
	char ca{ a };//error C2397: 从“int”转换到“char”需要收缩转换
	char cb{ 365 };//error C2397: 从“int”转换到“char”需要收缩转换
	char cc{ 22 };//ok
	return 0;
}
 ```

#### initializer_list

initializer_list对象中的元素是常量值，无法改变initializer_list对象中元素的值。拷贝或赋值一个initializer_list对象不会拷贝列表中的元素，而只是其引用。 

std::initializer_list的使用示例如下：

```c++
#include <iostream>
#include <initializer_list>
 
int main() 
{
    std::initializer_list<int> empty_list;
    std::cout << "empty_list.size(): " << empty_list.size() << '\n';//0
 
    // create initializer lists using list-initialization
    std::initializer_list<int> digits {1, 2, 3, 4, 5};
    std::cout << "digits.size(): " << digits.size() << '\n';//5
 
    // special rule for auto means 'fractions' has the
    // type std::initializer_list<double>
    auto fractions = {3.14159, 2.71828};
    std::cout << "fractions.size(): " << fractions.size() << '\n';//2
}
```

```c++
#include <iostream>
#include <vector>
#include <initializer_list>
 
template <class T>
struct S {
    std::vector<T> v;
    S(std::initializer_list<T> l) : v(l) {
         std::cout << "constructed with a " << l.size() << "-element list\n";
    }
    void append(std::initializer_list<T> l) {
        v.insert(v.end(), l.begin(), l.end());
    }
    std::pair<const T*, std::size_t> c_arr() const {
        return {&v[0], v.size()};  // copy list-initialization in return statement
                                   // this is NOT a use of std::initializer_list
    }
};
 
template <typename T>
void templated_fn(T) {}
 
int main()
{
    S<int> s = {1, 2, 3, 4, 5}; // copy list-initialization
    s.append({6, 7, 8});      // list-initialization in function call
 
    std::cout << "The vector size is now " << s.c_arr().second << " ints:\n";
 
    for (auto n : s.v)
        std::cout << n << ' ';
    std::cout << '\n';
 
    std::cout << "Range-for over brace-init-list: \n";
 
    for (int x : {-1, -2, -3}) // the rule for auto makes this ranged-for work
        std::cout << x << ' ';
    std::cout << '\n';
 
    auto al = {10, 11, 12};   // special rule for auto
 
    std::cout << "The list bound to auto has size() = " << al.size() << '\n';
 
//    templated_fn({1, 2, 3}); // compiler error! "{1, 2, 3}" is not an expression,
                             // it has no type, and so T cannot be deduced
    templated_fn<std::initializer_list<int>>({1, 2, 3}); // OK
    templated_fn<std::vector<int>>({1, 2, 3});           // also OK
}
```

参考：

 [std::initializer_list ](http://en.cppreference.com/w/cpp/utility/initializer_list)

### 宽字符

```c
#include<iostream>
#include<locale>

int main()
{
    //设置语言
	setlocale(LC_ALL, "zh-CN");
    //或者
	_wsetlocale(LC_ALL, L"zh-CN");
	wchar_t *p(L"hello 世界");
	std::wcout << p << std::endl;

	std::cin.get();
	return 0;
}
```

### 模板

#### 函数模板

函数模板可以用来创建一个通用的函数，以支持多种不同的形参，避免重载函数的函数体重复设计。它的最大特点是把函数使用的数据类型作为参数。

函数模板只有被使用时才编译。

函数模板的声明形式为：

```c++
template<typename或class 数据类型参数标识符1,...,typename 数据类型参数标识符n>

<返回类型><函数名>(参数表)

{
    函数体
}
```

示例：

```c++
#include<iostream>
using namespace std;
int add(int a, int b) {
	cout << "int: ";
	return a + b;
}

template<class T>
T add(T a, T b) {
	cout << typeid(T).name() << ": ";
	return a + b;
}

void main()
{
	cout << add(1, 2) << endl;//int: 3 原生函数优先于模板函数
	cout << add<int>(1, 2) << endl;//int: 3指名使用模板函数，模板参数为int
	cout << add('1', '2') << endl;//char: c
	cout << add(2.2, 1) << endl;//int: 3
	cout << add(2.2, 1.1) << endl;//double: 3.3
	cout << add(2, 1.1) << endl;//int: 3
	cout << add('a', 2) << endl;//int: 99
	cin.get();
}
```

#### 模板接口

模板不仅可以传入数据类型，还可传入函数，借此可以实现模板接口

```c++
template<class T>
void show(T data) {
	cout << data << endl;
}

//一般用F代表函数
template<class T, class F>
void opData(T data, F opt) {
	opt(data);
}

void main()
{
	opData("abc", show<const char*>);
	opData(5, show<int>);
	cin.get();
}
```

#### 默认模板参数

TO-DO

#### 可变参数函数模板

类型一致

```c++
#include<iostream>
#include<cstdarg>

using namespace std;

template<class T>
T add(int n, T t...)
{
	cout << typeid(T).name() << endl;
	va_list arg_ptr; //列表开头指针
	va_start(arg_ptr, n);//从n开始向后读
	T res(0);
	for (int i = 0; i < n; ++i) {
		res += va_arg(arg_ptr, T);	//根据数据类型取出数据
	}
	va_end(arg_ptr); //结束
	return res;
}

void main()
{
	cout << add(4, 1, 2, 3, 4) << endl;	 //int\n10
	cout << add(5, 1, 2, 3, 4, 5) << endl;	 //int\n15
	cout << add(5, 11.1, 12.2, 13.3, 14.5, 15.5) << endl; //double\n66.6
}
```

类型不一致

```c++
#include<iostream>

using namespace std;

void show() {//递归出口
	cout << endl;
}

template <typename First,typename... Rest>
void show(const First& first, const Rest&... rest) {
	cout << first << " ";
	show(rest...);// recursive call using pack expansion syntax  
}

void main()
{
	show(); //\n calls first overload, outputting only a newline  

	// these call the second overload, the variadic template,   
	// which uses recursion as needed.  
	show(10, 20);	//10 20  
	show(100, 200, 300);  //100 200 300
	show("first", 2, "third", 3.14159);  //first 2 third 3.14159
}
```

模拟实现printf

```c++
#include<iostream>

using namespace std;

void myprintf(const char* str) {
	cout << str;
}
template<typename T,typename... Args>
void myprintf(const char* str, T t,Args... args)
{
	while (str != nullptr && *str != '\0') {
		if (*str == '%' && *(str + 1) != '%') {
			cout << t;
			myprintf(str + 2, args...);
			return;
		}
		else if (*str == '%' && *(str + 1) == '%') {
			cout << '%';
			str +=2;
		}
		else {
			cout << *str++;
		}
	}
}

void main()
{
	printf("%dABCDEFG%s%c%%%fXXXX", 10, "1234", '0', 1234.5);
    //10ABCDEFG12340%1234.500000XXXX
	putchar('\n');
	myprintf("%dABCDEFG%s%c%%%fXXXX", 10, "1234", '0', 1234.5);
    //10ABCDEFG12340%1234.5XXXX
}
```

#### 模板元

简单的模板在特化时基本只包含类型的查找与替换，这种模板可以看作是“类型安全的宏”。 而模板元编程就是将一些通常编程时才有的概念比如：递归，分支等加入到模板特化过程中的模板 。

示例：

```c++
#include<iostream>
using namespace std;
// 主模板
template<int N>
struct data
{
	//递归
	enum { res = data<N - 1>::res + data<N - 2>::res };
};

template<>
struct data<1>
{
	enum{res = 1};
};

template<>
struct data<2>
{
	enum { res = 2 };
};

void main()
{

	int  num = 40;
	cout << data<40>::res << endl;//模板元，代码加速

	cin.get();
}
```

参看：

[C++模板元编程](http://www.cnblogs.com/liangliangh/p/4219879.html)

[C++ 模板元编程的应用有哪些，意义是什么？](https://www.zhihu.com/question/21656266)

### 命名空间

命名空间将全局作用域细分为独立的, 具名的作用域, 可有效防止全局作用域的命名冲突。举例来说, 两个不同项目的全局作用域都有一个类 `Foo`, 这样在编译或运行时造成冲突。 如果每个项目将代码置于不同名字空间中, `project1::Foo` 和 `project2::Foo` 作为不同符号自然不会冲突。

在头文件中使用匿名空间导致违背 C++ 的唯一定义原则 (One Definition Rule (ODR))。但是鼓励在 `.cc` 文件内使用匿名名字空间。

#### 匿名命名空间

编译器会在内部为匿名命名空间生成一个唯一的名字，并为该匿名命名空间生成一条using指令。 

在匿名命名空间中声明的名称也将被编译器转换，与编译器为这个匿名命名空间生成的唯一内部名称(即这里的__UNIQUE_NAME_)绑定在一起。这些名称具有internal链接属性，这和声明为static的全局名称的链接属性是相同的，即名称的作用域被限制在当前文件中，无法通过在另外的文件中使用extern声明来进行链接。

相对于C的static声明来说,可以在匿名的空间里面声明多个变量和函数,这样可以省去对每个变量和函数添加static声明。实质上匿名空间的功能跟static声明是一样的。另外static不能修饰class。

```c++
namespace {                             // .cc 文件中

// 名字空间的内容无需缩进
enum { kUNUSED, kEOF, kERROR };         // 经常使用的符号
bool AtEof() { return pos_ == kEOF; }   // 使用本名字空间内的符号 EOF

} // namespace
```

示例

```c++
#include<iostream>
using namespace std;

namespace { 
	int i = 256; 
}

namespace ns {

	namespace { 
		int i = 128; 
	}

	void func(void)
	{
		cout << "ns::func :" << endl;
		cout << "::i=" << ::i << endl;
		cout << "ns::i=" << i << endl;
	}
}

int main(void)

{
	cout << ::i << endl; //256
	cout << "i=" << i << endl;  //i=256
	cout << "ns::i=" << ns::i << endl;  //ns::i=128
	ns::func();	 //ns:func:\n ::i=256\n ns::i=128 
	return 0;
}
```

#### 具名命名空间规范

定义

```c++
// .h 文件
namespace mynamespace {
// 所有声明都置于命名空间中
// 注意不要使用缩进
class MyClass {
    public:
    ...
    void Foo();
};

} // namespace mynamespace

// .cc 文件
namespace mynamespace {

// 函数定义都置于命名空间中
void MyClass::Foo() {
    ...
}

} // namespace mynamespace
```

不要在名字空间 `std` 内声明任何东西, 包括标准库的类前置声明. 在 `std` 名字空间声明实体会导致不确定的问题, 比如不可移植. 声明标准库下的实体, 需要包含对应的头文件.

最好不要使用 `using` 关键字,防止命名空间污染 ，保证名字空间下的所有名称都可以正常使用。

在 `.cc` 文件, `.h` 文件的函数, 方法或类中, 可以使用 `using` 关键字。

在 `.cc` 文件, `.h` 文件的函数, 方法或类中, 允许使用名字空间别名。

```c++
//命名空间别名
namespace fbz = ::foo::bar::baz;
```

#### using与全局变量、局部变量

```c++
#include<iostream>

using namespace std;
int num = 10;

namespace data
{
	int num = 99;
	int numx = 10;
}

void main()
{
	int numx = 5;
	using namespace data;
	cout << numx << endl;//5 局部变量覆盖命名空间

	cout << ::num << endl;	//10 使用::表明为全局变量
	cout << data::num << endl; //99
	cin.get();
}
```

#### inline namespace

内联命名空间是C++11 之后提供的支持，主要为了对库的编写提供版本控制，语法为`inline  namespace  ns_name { declarations } ` ,定义ns_name内的声明在外部的{}间可见。

例子：

```c++
namespace A {
#if USE_INLINE_B
   inline
#endif
   namespace B {
      int foo(bool) { return 1; }
   }
   int foo(int) { return 2; }
}

int main(void) {
   return A::foo(true);
}



```

```c++
namespace std {

    namespace pre_cxx_1997 {
        // ...
    }
#if __cplusplus < 1997L // pre-standard C++
    using namespace pre_cxx_1997;
#endif

#if __cplusplus >= 1997L // C++98/03 or later
                         // (ifdef'ed out b/c it probably uses new language
                         // features that a pre-C++98 compiler would choke on)

    namespace cxx_1997 {
        // ...
    };
#  if __cplusplus == 1997L // C++98/03
    using namespace cxx_1997;
#  endif

#endif // C++98/03 or later

} // namespace std
```

参考：

[What are inline namespaces for?](https://stackoverflow.com/questions/11016220/what-are-inline-namespaces-for)

[Inline namespace definitions (C++11)](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/namespace_association.htm)

