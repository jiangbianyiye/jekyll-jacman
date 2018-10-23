C++引用

引用被看作变量的别名，在定义时必须初始化

### 左值引用

左值引用可以看作对象的别名，以下简称左值引用为引用。引用定义时必须初始化，且一旦创建无法指向别的对象，这种指向也无法释放。没有`void &`类型的引用，也没有指向空对象的引用。在c++中，无法用引用实现链表，树等数据结构。

常量引用限制不能通过该引用对对象进行修改操作：

```c++
#include <iostream>
#include <string>
 
int main()
{
    std::string s = "Ex";
    std::string& r1 = s;
    const std::string& r2 = s;
 
    r1 += "ample";           // modifies s
//  r2 += "!";               // error: cannot modify through reference to const
    std::cout << r2 << '\n'; // prints s, which now holds "Example"
}
```

当一个函数返回左值引用时，函数调用表达式就成为左值表达式，如下例：

```c++
#include <iostream>
#include <string>
 
char& char_number(std::string& s, std::size_t n)
{
    return s.at(n); // string::at() returns a reference to char
}
 
int main()
{
    std::string str = "Test";
    char_number(str, 1) = 'a'; // the function call is lvalue, can be assigned to
    std::cout << str << '\n'; //Tast
}
```

但是在返回函数内局部变量时，会引发运行时异常

```c++
#include<iostream>
using namespace std;
 
int &fun()
{
    //int xt = 10;
    //return xt;  
    //warning: reference to local variable 'x' returned [-Wreturn-local-addr]
    
    static int x = 10;
    return x; //ok
}
int main()
{
    fun() = 30;
    cout << fun(); //30
    return 0;
}
```

### 引用与指针

指针的引用

```c++
#include<iostream>
using namespace std;
 
void swap(char * &str1, char * &str2)
{
  char *temp = str1;
  str1 = str2;
  str2 = temp;
}
 
int main()
{
  char *str1 = "GEEKS";
  char *str2 = "FOR GEEKS";
  swap(str1, str2);
  cout<<"str1 is "<<str1<<endl; //str1 is FOR GEEKS
  cout<<"str2 is "<<str2<<endl; //str2 is GEEKS
  return 0;
}
```

不能定义指向引用的指针

```c++
#include<iostream>
using namespace std;

int main()
{
int x = 10;
int *ptr = &x;
int &*ptr1 = ptr; //error: cannot declare pointer to 'int&'
}
```

引用可能会间接的指向空对象

```c++
#include<iostream>
using namespace std;

int main()
{
int *ptr = NULL;
int &ref = *ptr;
cout << ref; //Segmentation Fault (SIGSEGV)
}
```

关于引用和指针的用途，参考[When do we pass arguments by reference or pointer?](https://www.geeksforgeeks.org/when-do-we-pass-arguments-by-reference-or-pointer/)

### 右值引用

右值引用（rvalue reference），是C++程序设计语言自C++11标准提出的一类数据类型。用于实现移动语义（move semantic）与完美转发（perfect forwarding）。右值引用能延长临时对象的生命周期。常左值应用虽然也能延长临时对象的生命周期，但不能通过其修改对象。

右值引用不能直接引用左值

```c++
#include <iostream>
#include <string>
 
int main()
{
    std::string s1 = "Test";
//  std::string&& r1 = s1;  // error: can't bind to lvalue
 
    const std::string& r2 = s1 + s1; //okay: lvalue reference to const extends lifetime
//  r2 += "Test";    // error: can't modify through reference to const
 
    std::string&& r3 = s1 + s1;  // okay: rvalue reference extends lifetime
    r3 += "Test";    // okay: can modify through reference to non-const
    std::cout << r3 << '\n';  //output: TestTestTest
}
```

当一个函数既有左值引用的重载，又有右值引用的重载时，传入右值引用实参会调用右值引用形式的重载



P471

### 移动语意

### 完美转发









### const与引用

### 

### 引用数组非法

