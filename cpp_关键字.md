# 关键字

## explict
- 用来限定 构造函数 只能显式调用，不能隐式调用
- 只能用来修饰单参数构造函数
- 因为 无参数 或 多参数 本来就只能显式调用
```c++
class A {
public:
    int v;
    // 可以隐式
    // A (int tmp): v(tmp) {}
    explicit A (int tmp): v(tmp) {}
}
int main() {
    // 隐式的做法 explicit的话不能这么搞 
    A a = 10;
    
    // explicit 限定显式
    A b(10);

    return 0;
}
```

## static  *重点*
- 生命周期
  - 从定义该static 变量开始，直到程序结束，一直是static
- 作用域
  - 局部变量
    - 定义该变量的函数块
  - 全局变量
    - 定义该变量的文件范围，文件作用域

- 修饰变量
  - 变量放在静态变量区
  - 只被初始化一次

- 修饰类
  - 被修饰的成员变量，被 全子孙 所有实例 共享，不占对象的空间
  - 被修饰的成员函数，被 全子孙 所有实例 共享
    - 没有this指针
    - 所以不能访问类的非静态成员，和外部函数。
  - 相当于，类域中的，全局变量

- 注意事项
  - 静态成员变量 可以作为成员函数的参数，普通成员变量不行
    - 因为静态成员变量 相当与 类中的 全局变量
```c++
class A {
public:
    static int s_var;
    int var;
    void f1(int i=s_var); // 可以的，static变量可以作为成员函数的参数
    void f2(int i=var); // 报错。 非静态的不能当参数，直接访问就行了
};
```
   - 静态成员变量的类型， 可以是这个类自己。 普通成员变量，只能是这个类自己的指针 / 引用
```c++
class A {
public:
    static A s_var; // 可以的，静态的自己, 相当于存了一个静态范本
    A var;  // 非静态不能存自己
    A *p;   // 自己的指针可以存
    A &var1;// 引用也行
};
``` 
  - 静态成员函数
    - 不能调用非静态的东西
      - 因为 静态成员函数没有this指针， 你调非静态的东西， 他不知道你调的哪一个
    - 不能声明成 virtual
      - 因为static 函数其实不属于谁。
      - 虚表指针需要 this 指针来访问， static函数没有this指针
      - *虚指针调用关系： this -> vptr -> vtable -> virtual function*
    - 不能声明成 const
      - 也是因为static 函数没有this 指针
    - 不能声明成 volatile

  - static 全局变量和普通全局变量
    - 相同
      - 都是存在静态区
    - 不同
      - 作用域
        - 普通
          - 作用于整个程序
        - static
          - 只作用与这个文件
      - static全局变量只初始化一次


## const 
- 作用
  - 修饰函数参数
    - 使传进来的参数 的值 不会被改变
  - 在类里面
    - 修饰成员变量
        - 对于每个对象，这个值不会变
        - 但是可以搞很多对象出来，所以对于类而言，这个值不是焊死的
        - 因此不要在定义类的时候，就把const 成员变量的值给定义死
    - 修饰成员函数
      - 不能修改成员变量的值
      - 只能访问成员变量
      - 不能调用非const 的成员函数，防止间接改掉东西
      - 就是个死活不能改东西的函数
```c++
class A {
public:
    int var;
    void const_fun(int tmp) const { // const 成员函数
        var = tmp; // 这是不行的，会改掉var. const 函数啥都不能改
        fun(tmp);  // 间接改也不行
    }
    void fun(int tmp) {
        var = tmp;
    }
};
```
### define 和 const 的区别
- 区别
  - 编译阶段
    - define 在编译预处理时， 替换
    - const 在编译阶段 确定值
  - 安全性
    - define 定义宏常量，没有数据类型，不能检查
    - const 有类型，能检查数据类型，安全
  - 内存占用
    - define 是个宏， 用几次 替换几次， 占用代码区的空间
    - const 就保存一份，占用静态区的空间
  - 调试
    - define不能调试，是替换的
    - const 可以调试
- const 优点
  - 有数据类型，可以检查，安全
  - 可以调试
  - 占空间少

## inline 内联函数
- 像普通函数一样， 但是是在被调用出展开代码，减少调用开销，提高效率
- 也不一定真的内联，编译器会看着办，如果inline太长了，他就不内联了
- inline 可以在 .h 里面被定义，被多个文件include, 也不会报错
- 与宏的区别
  - 宏在预编译时展开， 内联函数在编译时展开
  - 宏是简单的文本替换，不会检查参数类型，语句能否编译
  - 但内联函数是真的函数，会检查参数类型，语句语法
  - 但内联造成代码膨胀， 快是快了


## new, malloc, delete
- `new` <-> `delete`   ;  `malloc` <-> `free`
- `new` 和 `malloc` 的 不同
  - 申请时
    - new 不用指定空间，编译器会根据对象来算
    - malloc 要指定空间大小
  - new 底层也是 malloc实现的
  - 返回值
    - malloc 申请成功，返回该内存的指针 `void *`；失败返回NULL
    - new 申请成功，返回对象指针; 失败抛出bad_alloc 异常
  - 
## delete 和 delete[]
  - delete 原理
    - 先执行这个对象的析构函数
    - 再调用 `operator delete` 标准库函数，释放内存
  - `delete` 和 `delete[]` 的区别
    - `delete` 释放单个对象的空间， 只调用一次析构函数
    - `delete[]` 释放数组空间。数组中的每个成员都调用一次析构函数 

## struct
- C/C++ struct 区别
  - C 中只能定义数据类型
  - C++ 中 能定义成员函数，访问权限等

- C++中为何还要struct
  - 为了兼容C

- C++ 中 struct 和 class 异同
  - 同
    - 都可以继承， 定义private，成员函数等
  - 异
    - struct 默认 public, 继承也是public
    - class 默认private, 继承也是private
  - class 可以定义模板参数
  - struct 不可以搞模板