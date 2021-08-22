## 基础
- boot 启动
- bootstrap 鞋带， 缩写成boot
- pull oneself up by one's bootstraps. 
- 拽着鞋带把自己拉起来
- 比喻计算机启动很矛盾：必须先运行程序，才能启动，不启动又没办法运行程序
- 早期只能想尽办法把一小段程序塞进内存，才能启动计算机。
- 这个过程叫拉鞋带。后来就叫boot了
- 把BIOS刷到ROM里面，开机通过BIOS加载一切
- ROM == Read only memory 只能读，不能乱写，速度快

- 阮一峰说的顺序：
1. 硬件自检POST
2. Boot Sequence. 外部储存设备的排序，从哪个设备开始boot. 
   读该设备的前512字节，如果最后两个字节是0x55和0xAA就是能启动的，否则跳过
   这512字节就是主引导记录。MBR, Master boot record
   MBR有三个部分，告诉计算机到硬盘的哪里找OS
    1. 1-446字节，调用操作系统的机器码
    2. 447-510字节，分区表 Partition table
       一共64个字节，分成4项，每个16字节。所以最多分4个一级分区
    3. 511-512字节，主引导记录签名 0x55 0xAA
3. 硬盘启动
4. 操作系统
   kernel进入内存。linux kernel 加载成功后，第一个运行/sbin/init，产生init进程，pid 为 1， 其他进程都是他的后代
   Init 线程加载系统的各个模块，窗口程序，网络程序等。直至执行/bin/login程序，进入登录页面

## Basic Input Output System
- 硬件和os 之间的桥梁
- 装载操作系统
- 没有BIOS，OS就不能控制硬件
- 烧在主板上的闪存里面。每个主板都要专门开发BIOS
- 操作系统代码在硬盘上，开机的时候没办法直接把操作系统直接加载上去，要BIOS引导
- 开机自检 power-on self-test POST
  - 查一遍所有硬件都对不对
- 激活其他BIOS芯片
  - SCSI 和 显卡有自己的BIOS
  - SCSI 是
- 给OS提供一系列底层的Routines，以给硬件提供接口
- 初始化硬盘, 时钟
- 目前的 BIOS 大多有 1MB 到 2MB，所以会被存储在 闪存（Flash Memory）中

## BIOS 4大功能 by Yale
1. POST 在加载OS前，检查各个硬件都工作正常
2. Bootstrap Loader 装载OS的过程。如果有可以运行的OS的话，BIOS就会把控制权交给OS
3. BIOS  基本的输入输出。 在OS和硬件之间提供接口。
4. CMOS Setup.  配置硬件和参数，包括密码，时间等 (就是写BIOS内容)
   - CMOS芯片中存储的是BIOS设定值资讯，而BIOS芯片中存储的是BIOS程序。

## 启动步骤 by Yale
1. Power supply
   先供电。电源要花一点时间才会稳定，没稳定了直接跑会出事。
   在电源稳定之前，chipset一直会给CPU发reset信号，防止他直接跑起来
   一直到Power Good signal 来了，才会真的开始跑

2. 加载BIOS
   reset撤回之后， CPU就可以跑东西了，但是内存里这时候没东西给他跑
   所以这时候就是直接找BIOS ROM 里面的BIOS boot 程序
   地址都是FFFF0h, 在系统内存最后


3. POST 
   检查各个硬件都对不对，不对哔一声
   The BIOS performs the power-on self test (POST). If there are any fatal errors, the boot process stops. POST beep codes can be found in this area of the Troubleshooting Expert.

4. 找显卡
   找显卡自己的BIOS，跑他
   地址都在内存里C000h的位置
   显卡的BIOS会初始化显卡，显卡信息会比整个电脑的硬件信息先显示出来，
   
5. 找其他硬件的BIOS
   找其他硬件的ROM,看看有没有BIOS，有就加载了

6. 显示BIOS启动界面

7. BIOS 接着测别的东西
   memory count-up test

8. System inventory
   接着test有哪些硬件
   动态地判断硬件情况，设置参数

9.  Plug and Play
    看看有哪些plug and play的东西，在这时候加载

10. 显示系统配置的界面
    一闪而过罗列一下你的系统

11. Boot sequence
    search 一遍从哪个硬盘里启动，U盘还是光盘还是哪个硬盘

12. 找主引导记录 MBR, Master boot record

13. 装载OS.交换控制权给OS.

BIOS -> MBR -> Boot Loader -> Kernel -> OS

## 启动步骤
1. 检查CMOS 设置
2. load the interrupt handlers and device drivers
3. 初始化寄存器 及电源管理
4. 执行通电自检 power-on self-test
5. 显示系统设置
6. 判断那些硬件可以启动
7. intiate the bootstrap sequence 启动顺序

## 分步说明
1. 检查CMOS
-   检查CMOS芯片上的RAM里面的设置。 
-   CMOS就是complementary metal oxide semiconductor.
-   CMOS里面存有你的OS的相关信息。你的系统换了的话这里面的东西也要改的

2. load the interrupt handlers and device drivers 加载中断处理程序和设备驱动
-   interrupt handlers 是硬件和OS之间的桥梁 （中断调用）
-   可编程接口
-   BIOS 会通知 CPU 各种硬件设备的中断号，并提供中断服务程序。
-   软件可以通过调用 BIOS 中断对软盘驱动器、键盘及显示器等外围设备进行管理。
-   键盘按键的时候，信号传到 keyboard interrupt handler里， 告诉CPU这是什么，再转给OS
-   因为BIOS一直要从设备那里截取信号，所以会被加载到RAM里，跑快一点

3. 初始化寄存器
   1. CPU加电后，把所有寄存器的值设为默认值。
   -  除了CS寄存器的值改为0xFFFF,其他寄存器的值都为0
   -  根据CS 和 IP的值就可以找到指令的物理地址0xFFFF:0x0000，也就是0xFFFF0。
   2. CPU就开始执行在这个位置开始执行，这里存放的一条无条件跳转指令JMP，跳转到BIOS的真正启动代码处。
   3. 

4. 检查显卡能不能用
-  显卡一般自己有个BIOS, 初始化显存以及graphics processor.
-  如果显卡自己没有BIOS, 主板上就会有别的ROM里面存有这个信息，
  
5. power on self test
   1. BIOS检查是cold boot还是重启
-  检查内存地址0000:0472的值。
-  如果是1234h 就是重启，就跳过post检查; 否则就都是cold boot， 都要检查

   2. POST
-  对所有内存地址进行一遍读写
-  检查键盘鼠标
-  检查PCI 总线，检查所有PCI 卡。 
   -  Peripheral Component Interconnect 外设部件互连标准
   -  插显卡，网卡，声卡， 264 MB/s, 即插即用
-  POST 过程中有问题，就会哔哔你, 显示一行字，这一阶段的error都是硬件问题

6. 显示系统设置
-  显示如下信息
   -  处理器信息
   -  硬盘和其他drive
   -  内存
   -  BIOS版本和日期
   -  显示
   -  SCSI信息
      -  small computer system interface
      -  外设，包括磁盘、磁带、CD-ROM、可擦写光盘驱动器、打印机、扫描仪和通讯设备
      -  SCSI硬盘，接口速度快，主要用于服务器，硬盘转速快，缓存容量大，CPU占用率低，扩展性远优于IDE硬盘，并且支持热插拔

7. 按照boot sequence 装载硬盘OS