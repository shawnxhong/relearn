# 多线程

## 线程状态
- New
  - 已经创建，没有启动

- Runable
  - Ready 就绪
  - Running 运行中

- Blocked 
  - 阻塞状态

- Waiting
  - 等待其他线程(通知或中断)
  - 不会自己返回

- Time_waiting
  - 超时等待状态
  - 会在指定的时间内，自行返回

- Terminated
  - 终止，已经执行完毕

## 关键字
- synchronized
  - 保证被修饰的方法 / 代码块，任意时刻只有一个线程执行

  - 使用方式
    - 修饰实例方法
      - 给当前对象实例加锁
      - 进入同步代码之前，要获得该对象的锁
    
    - 修饰静态方法
      - 给当前类加锁
      - 进入同步代码块之前，获得当前类的锁
    
    - 修饰代码块
      - 进入前要获得锁

- volatile
  - 解决变量在多个线程之间的可见性
    - 我改了这个变量，别人立刻可以看到

  - volatile的变量，会直接在主存中读写
  - 确保代码执行顺序不变
    - 编译器/CPU 会对指令重排优化
    - 代码的执行顺序和写的可能不一样
    - volatile 禁止他自动重排优化
  - 使用条件
    - 对变量的写入操作 不依赖变量的当前值
    - 或 能确保只有单个线程 能更新这个变量的值
    - 或 该变量 没有包含在具有其他变量的不变式中

- synchronized 和 volatile的区别
  - volatile 是线程同步的轻量级实现，不需要加锁，性能比 synchronized好
  - volatile 只能修饰变量   synchronized 可以修饰方法和代码块
  - volatile 不阻塞 synchronized 会阻塞
  - volatile 只保证了数据的可见性，不保证原子性     synchronized保证可见性和原子性
  - volatile 解决变量在多个线程间的可见行   synchronized 解决资源在多个线程间的同步性


## 多线程方法

### Thread 类的方法
- `run()`
  - 在 `Runable` 接口中定义
- `start()`
  - 在`Thread` 类中定义

  - new 一个 Thread, 一开始处在new的状态
  - 调用`start()`方法，`run()`自动执行，线程启动

  - 通过调用`start()`，间接执行`run()` 才是多线程
  - 直接执行`run()`，则`run()`会被当成一个主线程下的普通方法执行，不是在一个Thread中执行，不是真的多线程

- `sleep()`
  - 在`Thread` 类中定义
  - 使当前线程暂停一下
  - 不会释放锁
  - 需要捕获 `InterruptedException` 异常，定义吵醒了要干嘛

- `join()`
  - 在`Thread`类中被定义
  - 阻塞调用该方法的线程
  - 直到当前线程执行完毕后，调用该方法的线程再继续执行
    - 主线程 main 创建并启动子线程 t
    - 子线程很慢，main可能比 t 结束得要早
    - 要保证顺序执行，要等子线程执行结束
    - 在main里，`t.join();`
    - 等 t 跑完，在走main 之后的东西
  - `t.join()` 就是 t join 主线程 

- `yield()`
  - yield 就是 让
  - 让掉当前线程 CPU 的时间片
  - 重新变成就绪状态
  - 重新竞争 CPU 的调度权
  - 时间片可能被别人拿走，也有可能还是到自己手上
  - 结合优先权设置
    - `thread1.setPriority(Thread.MIN_PRIORITY);`
    - `thread2.setPriority(Thread.MAX_PRIORITY);`

### Object类 方法
- `wait()`
  - 定义在Object 类中
  - 必须在`synchronized` 语句块里使用，就是必须手上有锁
  - 释放锁，让别的线程可以运行，当前线程进入等待池

- `notify()` && `notifyAll()`
  - 定义在Object 中
  - 必须在 `synchronized` 里面用， 手上有对象锁
  - `notify()` 会唤醒一个处于等待该 对象锁 的线程，然后把剩下的跑完
    - 退出 synchronized 代码块后，释放锁
    - notify()之后不要写太耗时的代码，人家等着来拿锁了

  - `notifyAll()` 就是喊一堆等着资源的线程， 到锁池里自己去抢

- `interrupt()`
  - 终止线程
  - 不能直接杀了他，否则他手上如果有锁就完蛋了
  - `interrupt()` 将该线程的中断标记 设为 true
  - 目标线程 通过检测`isInterrupted()`标志获取自身是否已中断
    - 如果目标线程处于等待状态，该线程会捕获到InterruptedException
    - 目标线程检测到isInterrupted()为true 或者捕获了InterruptedException都应该立刻结束自身线程；


## 线程池
- 3个好处
  - 降低资源开销
    - 重复利用创建好的线程，避免频繁创建销毁
  - 提高响应速度
    - 任务来了直接跑，不用创建线程
  - 好管理
    - 控制线程个数，统一管理，调优，监控

- `ThreadPoolExecutor` 就是个已经设计好的线程池
- ```java
  public ThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, TimeUnit unit
                    BlockingQueue<Runnable> workQueue, ThreadFactory threadFactory, RejectedExecutionHandler handler);
  ```
  - `corePoolSize` 核心线程数
    - 最多可以同时运行的线程数量
    - 有新的任务时，就会创建一个线程执行任务
    - 池中的线程数量 == corePoolSize 后，到达的任务进入阻塞队列
  
  - `maximumPoolSize` 最多能创建几个线程
    - 和上面不矛盾，上面是核心线程数
    - 这个是包括非核心线程 的总数
        - 优先创建核心线程
        - 核心线程数达到上限 && 阻塞队列满了， 才创建非核心线程

  - `keepAliveTime` 等待时间
    - 当线程池中的线程数量大于corePoolSize时
    - 如果一个线程空闲的时间达到keepAliveTime 就会终止，直到线程池中的线程数 不超过corePoolSize 
  - `unit` 时间单位
  - `workQueue` 阻塞队列
    - 储存等待执行的任务
  - `threadFactory`
    - 创建线程的工厂
  - `handler`
    - 拒绝处理任务时怎么办

### 向线程池提交任务
- 通过`execute()`方法 向线程池提交任务
    - 若 现在在跑的线程数量 < corePoolSize -> 创建核心线程，运行任务
    - 若 现在在跑的线程数量 >= corePoolSize -> 这个任务放入阻塞队列
    - 若 阻塞队伍已满 && 现在在跑的线程数量 < maximumPoolSize -> 创建非核心线程 运行任务
    - 若 阻塞队伍已满 && 现在在跑的线程数量 >= maximumPoolSize -> 抛出 RejectExecutionException 异常

### 关闭线程池
- `shutDown()` && `shutDownNow()`
- 遍历工作线程，每个都`interrupt()` 一下。如果线程没有设计响应中断，可能就停不掉
- `shutDown()`
    - 将线程池的状态 设置成 `SHUTDOWN`
    - 正在执行的任务继续执行
    - 没有执行的任务 `interrupt()`
- `shutDownNow()`
    - 将线程池的状态 设置成 `STOP`
    - 在跑的`interrupt()`
    - 没跑的直接返回