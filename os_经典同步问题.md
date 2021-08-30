
# 经典同步问题

## 1. 生产者 消费者问题

```c
typedef int semaphore;
semaphore mutex = 1;
void P1() {
    down(&mutex); // 要设计成原语
    // 临界区
    up(&mutex);
}

void P2() {
    down(&mutex);
    // 临界区
    up(&mutex);
}
```

**使用信号量实现生产者-消费者问题**
- 问题描述
  - 使用一个缓冲区来保存物品
  - 只有缓冲区没有满，生产者才可以放入物品
  - 只有缓冲区不为空，消费者才可以拿走物品。

- 缓冲区属于临界资源
  - 需要使用一个互斥量 mutex 来控制对缓冲区的互斥访问

- 为了同步生产者和消费者的行为，需要记录缓冲区中物品的数量。
- 数量可以使用信号量来进行统计
  - 需要使用两个信号量
    - empty 记录 空格子 的数量
      - 生产者进程中使用
      - empty 不为 0 时，生产者才可以放入物品
    - full 记录 有东西的格子 的数量
      - 消费者进程中使用
      - full 信号量不为 0 时，消费者才可以取走物品。

- 不能先 down(mutex) 再 down(empty)
- 否则 生产者 先锁 缓冲区 再 down(empty)
- 发现 empty = 0, 就去睡觉了
- 然后缓冲区就锁死了 消费者进不来 不会去叫醒 生产者

```c
#define N 100
typedef int semaphore;
semaphore mutex = 1;
semaphore empty = N; //空格子
semaphore full = 0;  

void producer() {
    while(TRUE) {
        int item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void consumer() {
    while(TRUE) {
        down(&full);
        down(&mutex);
        int item = remove_item();
        consume_item(item);
        up(&mutex);
        up(&empty);
    }
}
```


## 2. 哲学家进餐问题
![](https://cs-notes-1256109796.cos.ap-guangzhou.myqcloud.com/a9077f06-7584-4f2b-8c20-3a8e46928820.jpg)

五个哲学家围着一张圆桌，每个哲学家面前放着食物。哲学家的生活有两种交替活动：吃饭以及思考。当一个哲学家吃饭时，需要先拿起自己左右两边的两根筷子，并且一次只能拿起一根筷子。

下面是一种错误的解法，如果所有哲学家同时拿起左手边的筷子，那么所有哲学家都在等待其它哲学家吃完并释放自己手中的筷子，导致死锁。

```c
#define N 5

void philosopher(int i) {
    while(TRUE) {
        think();
        take(i);       // 拿起左边的筷子
        take((i+1)%N); // 拿起右边的筷子
        eat();
        put(i);
        put((i+1)%N);
    }
}
```

为了防止死锁的发生，可以设置两个条件：

- 必须同时拿起左右两根筷子；
- 只有在两个邻居都没有进餐的情况下才允许进餐。

```c
#define N 5
#define LEFT (i + N - 1) % N // 左邻居
#define RIGHT (i + 1) % N    // 右邻居
#define THINKING 0
#define HUNGRY   1
#define EATING   2
typedef int semaphore;
int state[N];                // 跟踪每个哲学家的状态
semaphore mutex = 1;         // 临界区的互斥，临界区是 state 数组，对其修改需要互斥
semaphore s[N];              // 每个哲学家一个信号量

void philosopher(int i) {
    while(TRUE) {
        think(i);
        take_two(i);
        eat(i);
        put_two(i);
    }
}

void take_two(int i) {
    down(&mutex);
    state[i] = HUNGRY;
    check(i);
    up(&mutex);
    down(&s[i]); // 只有收到通知之后才可以开始吃，否则会一直等下去
}

void put_two(i) {
    down(&mutex);
    state[i] = THINKING;
    check(LEFT); // 尝试通知左右邻居，自己吃完了，你们可以开始吃了
    check(RIGHT);
    up(&mutex);
}

void eat(int i) {
    down(&mutex);
    state[i] = EATING;
    up(&mutex);
}

// 检查两个邻居是否都没有用餐，如果是的话，就 up(&s[i])，使得 down(&s[i]) 能够得到通知并继续执行
void check(i) {         
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] !=EATING) {
        state[i] = EATING;
        up(&s[i]);
    }
}
```

## 3. 读者-写者问题

允许多个进程同时对数据进行读操作，但是不允许读和写以及写和写操作同时发生。

一个整型变量 count 记录在对数据进行读操作的进程数量，一个互斥量 count_mutex 用于对 count 加锁，一个互斥量 data_mutex 用于对读写的数据加锁。

```c
typedef int semaphore;
semaphore count_mutex = 1;
semaphore data_mutex = 1;
int count = 0;

void reader() {
    while(TRUE) {
        down(&count_mutex);
        count++;
        if(count == 1) down(&data_mutex); // 第一个读者需要对数据进行加锁，防止写进程访问
        up(&count_mutex);
        read();
        down(&count_mutex);
        count--;
        if(count == 0) up(&data_mutex);
        up(&count_mutex);
    }
}

void writer() {
    while(TRUE) {
        down(&data_mutex);
        write();
        up(&data_mutex);
    }
}
```

## 管程

- c 语言不支持管程
- Pascal支持管程 提供了 insert() 和 remove() 方法
- 管程
  - 在一个时刻只能有一个进程使用管程
  - 进程 无法继续执行时 要让出来

管程引入了*条件变量*以及相关的操作：*wait()*和 *signal()* 来实现同步操作
对条件变量执行 wait() 操作会导致调用进程阻塞，把管程让出来给另一个进程持有。
signal() 操作用于唤醒被阻塞的进程。

**使用管程实现生产者-消费者问题**

```pascal
// 管程
monitor ProducerConsumer
    condition full, empty;
    integer count := 0;
    condition c;

    procedure insert(item: integer);
    begin
        if count = N then wait(full);
        insert_item(item);
        count := count + 1;
        if count = 1 then signal(empty);
    end;

    function remove: integer;
    begin
        if count = 0 then wait(empty);
        remove = remove_item;
        count := count - 1;
        if count = N -1 then signal(full);
    end;
end monitor;

// 生产者客户端
procedure producer
begin
    while true do
    begin
        item = produce_item;
        ProducerConsumer.insert(item);
    end
end;

// 消费者客户端
procedure consumer
begin
    while true do
    begin
        item = ProducerConsumer.remove;
        consume_item(item);
    end
end;
```