# 单例模式
- 保证一个类仅有一个实例
- 提供一个访问他的全局访问点
- 该实例对所有人可见
## 创建方式
1. 构造函数private, 防止别人创建
2. private static 指针 指向唯一的实例
3. 一个public static 方法获取该实例

## 懒汉版
- 第一次被使用时才初始化，延迟初始化
```c++
class Singleton {
private:
    static Singleton* instance;
    Singleton() {};
    ~Singleton() {};
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
public:
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
            return instance;
        }
    }
};

Singleton * Singleton::instance = nullptr;
```