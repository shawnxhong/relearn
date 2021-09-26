# 容器
- 容器最顶级的接口是 `Iterable`， 使所有容器具备迭代元素的功能
  - `Collection` 接口， 继承了`Iterable`接口，统一定义了单列容器
    - `List` 接口
      - 线性表
        - `ArrayList`
          - 数组
        - `LinkedList`
          - 双链表
        - `Vector`
          - 同步的线程安全的数组
      
    - `Map` 接口
      - 键值对容器
      - `HashMap`
        - 非线程安全
      - `HashTable`
        - 线程安全， 大部分都synchronized过
      - `TreeMap`
        - 红黑树

    - `Set` 接口
      - 集合，互不相同
      - `HashSet`
        - 基于`HashMap`实现
      - `TreeSet`
        - 基于`TreeMap`

- `Iterator`接口，迭代器，每个容器都实现迭代器

- `Iterable` 的方法
  - `iterator()`
    - 返回一个迭代器实例
  - `forEach()`
    - 对每个元素做一个什么操作
  - `spliterator`
    - 并行遍历所有元素

- `Iterator` 的方法
  - `hasNext()`
    - 后面还有伐
  - `next()`
    - 后移一位 返回
  - `remove()`
    - 删除迭代器指向的元素
    - 如果这个东西不能删， 抛出`UnsupportedOperationException` 异常
  - `forEachRemaining()`
    - 对迭代器以后剩余的的东西 干个什么操作

- `Collection` 的方法
  - `add()`
    - 添加一个元素
  
  - `addAll()`
    - 将另一个容器里的元素 全部添加到该容器里
    - 并集运算
  
  - `remove()`
    - 删一个
  
  - `removeAll()`
    - 给另一个容器，取差集
  
  - `clear()`
    - 清空本容器
  
  - `retainAll()`
    - 和另一个容器， 取交集
  
  - `contains()`
    - 是否包含某一元素
  
  - `containsAll()`
    - 判断是否是另一个容器的超集
  
  - `isEmpty()`
    - 是否空
  
  - `size()`
    - 元素数目
  
  - `toArray()`
    - 转化成`Object` 数组 并返回

- `List` 方法
  - `ArrayList`, `LinkedList`, `Vector`
  - 有`Collection` 的一切方法`
  - `set()`
    - 修改指定下标元素
    - 不能像c++ 一样直接弄下标
  - `get()`
    - 取那个下标的元素
  - `indexOf()`
    - 从左边开始搜这个元素的下标
  - `lastIndexOf()`
    - 从右边开始搜这个元素的下标
  - `subList()`
    - [l,r) 取连续子序列
  - `listIterator()`
    - 取迭代器，可以指定位置

## ArrayList, LinkedList, Vector
- ArrayList
  - 数组
  - 尾部插入删除O(1)， 别的要O(n)挪动
  - 没锁， 线程不安全

- Vector
  - 和ArrayList 方法一样，区别是有锁，线程安全。
  - 效率肯定低一点

- LinkedList
  - 双链表
  - 查找慢
  - 没锁，线程不安全

## Map Set
- `Map` 的方法 
  - `put()`
    - 插入键值对
    - 没有就插入，有就更新
  - `putAll()`
    - 把另一个Map 的所有键值对都弄进去
  
  - `remove()`
    - 给个key, 删了这个key-value
  - `clear()`
    - 全删光
  
  - `containsKey()`
    - O(1)
  - `containsValue()`
    - 慢
  
  - `get()`
    - 给key 取 value
  - `entrySet()`
    - 返回所有 key-value 的集合
  - `keySet()`
    - 返回所有 key 的集合

  - `values()`
    - 返回所有values 的容器
  
  - `isEmpty()`
  - `size()`
  
### `HashMap` && `HashTable`
- `HashMap` 就是 unordered_map
  - 链表法解决冲突
  - 链表长度 > 8 时 转化为红黑树
  - 允许 key || value 为 null
  - 不是线程安全的，速度快点

- `HashTable`
  - 线程安全，大部分关键字 synchronized 过
  - 不允许key 或 value 为null
  - 不转 红黑树

- `TreeMap`
  - 就是一个大红黑树


### `HashSet` && `TreeSet` 
- `HashSet`
  - 基于`HashMap`实现
  - 通过key 的 `hashCode()` 检查冲突
    - 如果有冲突，再 `equals()`看下是不是真的是同一个东西

- `TreeSet`
  - 基于`TreeMap`实现
  - 可以使用`Comparable`接口 或 `Comparator`接口 排序元素