
## 1.前言

new是C++程序设计语言中的一种语言结构，用于动态分配内存、并用构造函数初始化分配的内存。

new的使用称为“new运算符表达式”，其内部实现分为两步：

1. **调用相应的operator new()函数，动态分配内存。**如果operator new()不能成功获得内存，则调用new_handler函数。如果没有设置new_handler函数或者new_handler未能分配足够内存，则抛出std::bad_alloc异常。“new运算符表达式”所调用的operator new()函数，按照C++的名字查找规则，首先做依赖于实参的名字查找（即ADL规则），在要申请内存的数据类型T的内部、数据类型T定义处的命名空间查找；如果没有查找到，则直接调用全局的::operator new()函数。

2. **在分配到的动态内存块上初始化相应类型的对象并返回其首地址。**如果调用构造函数初始化对象时抛出异常，则自动调用operator delete()函数释放已经分配到的内存。

* 每个new获取的对象，必须用delete析构并释放内存，以免内存泄漏。

* new运算符表达式是C++的一种语言结构，不可重载。但用户可重载operator new()函数。


##2.new运算符表达式

###2.1 普通的new运算符

model：

``` c++
p = new typename;
p = new typename(init);
p = new typename{init};
``` 

eg：

```c++
int *p = new int;
int *p = new int(5);
int *p = new int{5};//C++ 11
```

###2.2 生成对象数组的new运算符
model:
```cpp
p = new type[size];
p = new type[size] {one, two , size};
```

eg:
```cpp
p = new int[5];
p = new int[5]{1, 2, 3, 4, 5};
```
###2.3 带位置的new运算符

model:
```cpp
new ( expression-list ) type ( init);
```
expression-list将作为`operator new()`函数的实参列表的结尾部分。这种形式的new运算符表达式首先调用`operator new(size_t,OtherTypeList)`函数来获取内存；然后对该对象执行构造函数。这里的OtherTypeList作为形参列表要和new括号里的实参列表expression-list的类型兼容（即形参实参能够匹配）。

带位置的new运算符，语义上包括四种使用情形：

1. 直接给出要构建的对象的内存位置；
2. 不抛出异常，如果内存分配失败返回空指针；
3. 定制的、带其他参数的内存分配器；
4. 用于调试目的，在构造函数调用失败时给出源文件名与行号。

狭义上的带位置的new是指第一种情形。使用这种placement new，**原因之一**是用户的程序不能在一块内存上自行调用其构造函数（即用户的程序不能显式调用构造函数），必须由编译系统生成的代码调用构造函数。**原因之二**是可能需要把对象放在特定硬件的内存地址上，或者放在多处理器内核的共享的内存地址上。

释放这种对象时，不能调用placement delete，应直接调用析构函数，如：pObj->~ClassType();然后再自行释放内存。

```cpp
#include <iostream> 
using namespace std; 
int main(int argc, char* argv[]) {
    char buf[100];
    int *p=new (buf) int(101); 
    cout<<*(int*)buf<<endl;
    return 0; 
}
```

###2.4 保证不抛出异常的new 运算符

在分配内存失败时，new运算符的标准行为是抛出std::bad_alloc异常。也可以让new运算符在分配内存失败时不抛出异常而是返回空指针。其中`nothrow`是`std::nothrow_t`的一个实例.

```
p = new (nothrow) type(init);
p = new (nothrow) type[size];
```

###2.5 自行定制参数的new运算符表达式

new运算符的参数可以是任意合法类型的列表。由C++的重载机制来决定调用那个operator new。
```cpp
new (Type_list) Type ( init );
new (Type_list) Type[size];
```

###2.6 带位置的delete运算符

C++ 不能使用带位置的 delete 运算符表达式直接析构一个对象但不释放其内存。因此，对于用广义的带位置new表达式构建的对象，析构释放时有两种办法：

第一种办法是直接写一个函数，完成析构对象、释放内存的操作 。


##3.operator new() 函数重载

###3.1普通的operator new(size_t size)函数

operator new函数可以被每个C++类作为成员函数重载。也可以作为全局函数重载。

```cpp
void * operator new (std::size_t) throw(std::bad_alloc);
void operator delete(void*) throw();
```

内存需要回收的话,调用对应的operator delete()函数。

例如，在new运算符表达式的第二步，调用构造函数初始化内存时如果抛出异常，异常处理机制在栈展开（stack unwinding）时，要回收在new运算符表达式的第一步已经动态分配到的内存，这时就会自动调用对应operator delete()函数。

###3.2数组形式的operator new  \[]\(size_t size)函数

```
void * operator new[] (std::size_t) throw(std::bad_alloc); 
void operator delete[](void*) throw();
```

###3.3 带位置的operator new函数void* operator new(size_t,void*)

`operator new(size_t,void*)`函数用于带位置的new运算符调用。C++标准库已经提供了operator new(size_t,void*)函数的实现，包含`<new>`头文件即可。这个实现只是简单的把参数的指定的地址返回，带位置的new运算符就会在该地址上调用构造函数来初始化对象。


```cpp
// Default placement versions of operator new. 
inline void* operator new(std::size_t, void* __p) throw() { return __p; } 
inline void* operator new[](std::size_t, void* __p) throw() { return __p; } 
// Default placement versions of operator delete. 
inline void  operator delete  (void*, void*) throw() { }
inline void  operator delete[](void*, void*) throw() { }
```

禁止重定义这4个函数。因为都已经作为`<new>`的内联函数了。在使用时，实际上不需要`#include <new>`

对应的placement delete函数，只应在placement new运算符表达式在第二步调用构造函数抛出异常时被异常处理机制的栈展开操作自动调用。

###3.5保证不抛出异常的operator new函数

C++标准库的`<new>`中还提供了一个nothrow的实现，用户可写自己的函数替代：

```cpp
void* operator new(std::size_t, const std::nothrow_t&) throw(); 
void* operator new[](std::size_t, const std::nothrow_t&) throw(); 
void operator delete(void*, const std::nothrow_t&) throw(); 
void operator delete[](void*, const std::nothrow_t&) throw();
```

###3.6自行定制参数的operator new函数

这种函数被自行定制参数的new算符调用。需要由用户自行定义，以确定分配内存时的行为.

```cpp
operator new(size_,Type1, Type2, ... );
```
eg:
```cpp
char data[1000][sizeof(int)]; 

inline void* operator new(size_t ,int n) ;{        
    return data[n]; 
} 
void foo(){    
    int *p=new(6) int(102); //把整型对象创建在data的第六个单元上 
}
```
