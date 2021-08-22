# 语言对比
## C++11特性
1. auto 类型推导 
   - 在*编译期* 通过初始值，推导出变量类型。auto 出来的必须有初始值。
   - `auto var =  v1 + v2; // 根据v1 v2的类型推导出var是个啥`

2. decltype 类型推导
   - decltype == declare type
   - 声明类型
   - 也是用来在编译期 做自动类型推导
   - 从表达式中 推断出要定义的变量的类型 
```C++
auto var = v1 + v2; // 根据v1 v2 推导出 var, 并将初始值赋值给var
decltype(var1 + val2) v = 0; // decltype 根据var1 + var2 推导出 v的类型， 变量初始值和表达式的值 没关系。
// 初始值是 = 后面的
```

3. lambda 表达式
```c++
// 匿名函数的定义
[capture list] (parameter list) -> return type {
    function body
};
// 举例
int main() {
    int arr[4] = {4,3,2,1};
    // 升序排序
    sort(arr, arr+4, [=] (int x, int y) ->bool { // [=] 就是默认以值捕获所有变量
        return x<y;
    });
    return 0;
}
```
4. 范围 for 语句
```c++
char arr[] = "hello world!";
for (char c: arr) {
    cout << c;
}
```

5. 右值引用
给临时值一个地址
```c++
int var = 42;
int &left_var = var; // 左值引用
// int &&right_var = var; // error, 左值不能弄右值引用
int &&right_var = var + 40; // ok, 右值引用绑右值
```
6. 标准库 move() 函数
在<utility>头文件里，把右值转到左值

7. 智能指针
前面讲了

8. delete 和 default 函数
- delete 函数
  - `= delete` 表示该函数不能被调用 
- default 函数
  - `= default` 表示编译器生成默认的函数，如默认的构造函数
```c++
class A {
public:
    A() = default;  // 使用默认构造函数
    ～A() = default; // 默认析构函数
    A(const A &) = delete  // 生成的对象不能拷贝构造
    A &operator=(const A &) = delete; // 类的对象禁止拷贝赋值
};

int main() {
    A a1;
    A a2 = a1; // 不能拷贝构造
    A a3;
    a3 = a1; // 不能拷贝赋值
}
```

## C 和 C++ 区别
1. 面向对象和面向过程
- 面向过程
  - 分析解决问题所需的步骤，依次实现
- 面向过程
  - 把构成问题的事务，分解为各个对象
  - 建立对象的目的，不是完成一个步骤，而是描述某个事务在解决整个问题步骤中的行为

2. C / C++ 的区别
- 语言层面
  - C 面向过程。主体是函数。通过main调用各个子函数。运行顺序人为操控好
  - C++ 面向对象。主题是对象。main函数 先定义一些类，根据需要执行类的成员函数。
    - 将涉及到的数据和操作封装在类里。以事件或消息驱动对象来处理

- 应用领域
  - C 主要写嵌入式，驱动开发等硬件领域。
  - C++ 可以写应用层开发，用户界面开发，OS

- C++ 
  - 操作底层
  - 有oop
  - 类型检查更严格
  - 泛型编程
  - 异常处理
  - 运算符重载
  - STL
  - 命名空间

## Java 和 C++ 区别
- 语言特性
  - 指针
    - C++ 可以操作指针，会产生内存泄漏，非法引用等问题
    - Java语言层面没有指针。指针都在JVM内部，人不能直接操作
  - 多重继承
    - c++可以多重继承，一个类可以有好几个爹。   毛病：菱形继承
    - java不能多重继承。 一个类可以继承多个interface，间接多重继承，还没毛病。
  - 数据类型和class
    - java 完全面向对象。所有函数和变量必须是类的一部分。除了基本数据类型 int 这些，其他所有东西都是类，包括数组
    - c++ 可以有全局函数，变量
- 垃圾回收
  - java 有GC 机制，内存管理jvm管
  - java 所有对象都建立在堆栈上，jvm利用空闲时间，用一个线程管GC

- 应用场景
  - java 
    - 运行在 jvm 上，和开发平台无关
    - 主要用来开发web 应用
  - c++ 
    - 编译成exe, 能否跨平台取决于编译器特性
    - 嵌入式，网络，并发编程

## Python 和 C++ 区别
- 语言层面
  - python 为脚本语言，解释执行，不需要编译
  - C++ 需要编译

- 运行效率
  - c++ 效率高，编译的 exe 直接就是机器码，CPU 指令也比python 少
  - python 通过C 实现，转换出来的机器码要经过好几层，CPU 指令更多。 和CPU 之间还隔了个解释器，不是运行机器码

- 开发效率
  - python 比c++ 好写多了

- 书写格式和语法
  - Python 不需要声明变量类型，强制需要缩进， 没有花括号