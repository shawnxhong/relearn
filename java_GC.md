# 垃圾回收
- `java.lang.System.gc`，等价于`java.lang.Runtime.getRuntime.gc`
- 提示 jvm 进行垃圾回收，由系统调用，不用手动

- `finalliz()`
  - 在对象被杀掉前要调用的方法
  - 遗言
  - 默认没有
  - 要有的话自己重写

## 引用计数
- jvm 里一般不用引用计数
- 循环引用不用weak pointer不行

### 四种引用
1. 强引用
- `Object obj = new Object();` 这样的传统引用
- 只要这样被引用的对象都不会被GC

2. 软引用
- 还有用，但非必须的对象
- 内存够的时候不会被GC
- 内存不够了就会被GC

```java
// 强引用
String strongReference = new String("abc");
// 软引用
String str = new String("abc");
SoftReference<String> softReference = new SoftReference<String>(str);
```

3. 弱引用
- 比软引用还弱
- 只要GC 了，就会被回收
- `WeakReference`

4. 虚引用
- 不会给对象续命
- 也拿不到对象实例
- 只是为了GC时，收到一个系统通知

## 可达性分析
- Reachability analysis
- 从 GC roots 的根节点开始dfs 遍历
- 走过的路径叫 reference chain 
- 孤立的节点 就是没人再用的
  - 就会被全部杀掉

### GC roots
- 虚拟机栈中引用的对象
  - 被线程调用的方法堆栈中用到过的
    - 参数
    - 局部变量
    - 临时变量
- 类static 属性 引用的对象

- 方法区中，常量引用的对象
  - 字符串常量池里的引用

- 本地方法区中 native 方法引用的对象

- JVM 内部的引用
  - 基本数据类型对应的class
  - 常驻的一场对象
  - 系统类加载器

- 被同步锁持有的对象 synchronized

- 根据分代收集 和 局部回收 还会有别的GC roots

## GC 算法
- **标记-清除算法**
  - 基本款
  - 先O(n)标记一遍谁要被杀掉
  - 再一个一个杀
  - 缺点
    - 太慢了
    - 清除后会产生大量碎片

- **复制算法**
  - 把内存 一分为二
  - 每次只用其中的一块
  - 用完一块后，将活的对象 排整齐移到另一边，原来那边全清空
    - 挪到新地盘的时候，只要顺序移动指针就行了，简单
  - 优点
    - 简单，没碎片
  - 缺点
    - 内存只剩一半
    - 如果活的多，挪来挪去效率差

- **标记-整理算法**
  - 基于老年代
  - 线性标记要杀的
  - 把不要杀的都移到内存的一端
  - 边界以外的全杀掉
  - 开销大，会造成停顿stop the world

- **分代收集算法**
  - 分为多个年龄区，每个区的GC算法都不一样
  - 基于三个假说
    - 弱分代假说 Weak Generational Hypothesis
      - 绝大多数对象 都是很快就要被杀掉的 (98%)
    - 强分代假说 Strong Generational Hypothesis
      - 熬得越久越难死
    - 跨代引用假说 intergenerational reference hypothesis
      - 跨代引用很少，绝大部分都是同代引用
    - 年龄为经历过的GC 次数
  
  - 朝生夕灭的集中放一堆
    - 每次只要关注活得下来的那几个，剩下的都直接杀了

  - 老不死的集中放一堆
    - 这堆就不用每次去看了
  
## 细说分代收集
- Heap 分为新生代 老生代
  - 新生代
    - Eden 区
    - From Survivor 区
    - To Survivor 区 (复制算法)
    - Eden : Survivor = 8 : 1 : 1
  - 老生代

- *Minor GC*
  - 新生代的GC
  - 频繁执行，速度很快
  - 复制算法
    - 每次只使用Eden 和一块Survior
    - GC 时， 活下来的，放到另一块 Survivor里，Eden和被清的Survivor全清掉
  - 
- *Full GC/ Major GC*
  - 会伴随至少一次Minor GC
  - 很少执行，速度慢
  - 标记清除 / 标记整理

- 对象优先分配在 Eden区
  - 当Eden区 空间不够时，发起Minor GC

- 大对象直接进入老年代
  - 需要连续内存的大对象
    - 巨型String
    - 巨型数组
  - 避免大对象从Eden 复制到老年区，慢的要死

- 熬过几次GC的进入老年区
  - 每个对象都有年龄counter
  - Eden 里的东西，一次GC后，进入Survior区，年龄++
  - 在Survivor区，每憋过一次GC, 年龄++， 15岁进老年区
  - 或，Survivor区 一半以上对象年龄到达 x， 年龄 > x的进老年区

- 空间分配担保
  - MinorGC 之前，看一下 老年代 最大连续空闲空间 是否大于 新生代所有东西的空间总和
    - ok 的话 说明 MinorGC 完之后塞的下， 直接Minor GC
  - 否则Full GC, 老年代也要打扫一下