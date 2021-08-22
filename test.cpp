# include <iostream>
using namespace std;

class A{
public:
    int a, b;
    int add(){
        return a + b;
    }
};

int main() {
    A obj;
    obj.a = 3, obj.b = 4;
    int *p = &obj.a;    // 指向对象的 成员变量

    int (A::*fun_p)();  // 指向对象的 成员函数
    fun_p = &A::add;
    cout << (obj.*fun_p)();
    return 0;
}