# 1. Theoretical Foundation

## 1.1 Introduction

serial computing: serial implementation of programs for single CPUs 
parallel computing: program is executed using multiple machines

why parallel computing：

- physical/parctial constraints for even fast serial computers
  - transmissin speeds - the speed of a serial computer is directly dependent upon how fast data  case move through hardware.
  - limits to miniaturization - processor technology is allowing an increasing number of transitors to be place on a chip.
- econmics limitations - increasingly expensive to make a single processor fater. using a larger number of moderately fast commodity processors to achive the same performance is less expensive.
- energy limt - limits imposed by cooling needs for chips and supercomputers.

最开始人们用的是single-core的processor。但是发现这种single-core processor会遇到power wall
After hitting the wall, people started to thing multi-core processor.
multi-core processor can also help us conduct parallel programming

Therefore, after that, all major processor vendors are producing multicore chips, giving rise to that every machine is already a parallel machine

**Parallel computing concepts and terminology:**

- Terminology: units of measure
  - Flop: floating point operation, usually double precision unless noted.
  - Flop/s ("flops"): floating point opeartions per second.
  - Bytes: size of data (a double precision floating point number is 8 bytes).

- von Neumann Architecture
  - memory: RAM (radom access memory) stores both program instructions and data
    - program instructions are coded data which tells the computer to do something
    - data is simply information to be used by the program.
  - control units - fetch instructions/data from memory, decodes the instructions and then sequentially coordinates operations to accomplish the programmed task.
  - arithmetic units - perform basic arithmetic operations
  - input/output - is the interface to the human operator

Cluster: A set of computers connected over a Local Area Network (LAN) that functions as a single large multiprocessor.

Multicores, multiprocessors:

- Multiple processors per chip.  
  • Processors are referred to as cores.    【所以是同一个概念】
    → Number of cores per chip is expected to double per year.  【moole's law】

**Performance evalutions:**
References
[1](https://blog.csdn.net/rs_gis/article/details/115860381) [2](https://blog.csdn.net/Cobb141/article/details/106994450?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-106994450-blog-83239493.pc_relevant_multi_platform_whitelistv3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-1-106994450-blog-83239493.pc_relevant_multi_platform_whitelistv3&utm_relevant_index=2)

- [cache (缓存)](https://baike.baidu.com/item/%E7%BC%93%E5%AD%98/100710?fr=aladdin)：
  - 原始意义是指访问速度比一般随机存取存储器（RAM）快的一种高速存储器，通常它不像系统主存那样使用DRAM技术，而使用昂贵但较快速的SRAM技术。缓存的设置是所有现代计算机系统发挥高性能的重要因素之一。
  - 缓存是指可以进行高速数据交换的存储器，它先于内存与CPU交换数据，因此速率很快。
  - 缓存的工作原理是当CPU要读取一个[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)时，首先从[CPU缓存](https://baike.baidu.com/item/CPU%E7%BC%93%E5%AD%98/3728308?fromModule=lemma_inlink)中查找，找到就立即读取并送给CPU处理；没有找到，就从速率相对较慢的[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)中读取并送给CPU处理，同时把这个[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)所在的[数据块](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%9D%97?fromModule=lemma_inlink)调入缓存中，可以使得以后对整块数据的读取都从缓存中进行，不必再调用内存。正是这样的读取机制使CPU读取缓存的命中率非常高（大多数CPU可达90%左右），也就是说CPU下一次要读取的[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)90%都在CPU缓存中，只有大约10%需要从[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)读取。这大大节省了CPU直接读取[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)的时间，也使CPU读取[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)时基本无需等待。总的来说，CPU读取[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)的顺序是先缓存后[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)。
  - 缓存只是[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)中少部分[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)的复制品，所以CPU到缓存中寻找数据时，也会出现找不到的情况（因为这些数据没有从内存复制到缓存中去），这时CPU还是会到[内存](https://baike.baidu.com/item/%E5%86%85%E5%AD%98?fromModule=lemma_inlink)中去找[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)，这样系统的[速率](https://baike.baidu.com/item/%E9%80%9F%E7%8E%87?fromModule=lemma_inlink)就慢下来了，不过CPU会把这些数据复制到缓存中去，以便下一次不要再到内存中去取。随着时间的变化，被访问得最频繁的[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)不是一成不变的，也就是说，刚才还不频繁的数据，此时已经需要被频繁的访问，刚才还是最频繁的[数据](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE?fromModule=lemma_inlink)，又不频繁了，所以说缓存中的数据要经常按照一定的[算法](https://baike.baidu.com/item/%E7%AE%97%E6%B3%95?fromModule=lemma_inlink)来更换，这样才能保证缓存中的数据是被访问最频繁的。
- concurrency(并发性)；
- what is HPC?   What is MPI?

## 1.2 High performance computing (HPC) principles, metrics, and models

**Outline: 

- principles of hpc
- the roofline model
- cache hierarchy

**PRINCIPLES OF HPC**

architectural metrics

arithmetic (operational) intensity: ratio between its total work to its memory operations.

> 有个叫运算强度（Arithmetic intensity）的概念，即flops per byte，表示一个字节的数据上发生的运算量，只要这个运算量足够大，意味着传输一个字节可以消耗足够多的计算量，

**THE ROOFLINE MODEL**
the roofline model allows us to:

- estimate how efficient is our code, based on a given computational platform (esp. multicore CPUs and GPUs).
- estimate how much more performance to obtain if we optimize it.
- compare different systems and evaluate which one is best for our code.

下面这个参考内容讲的很好：https://zhuanlan.zhihu.com/p/34204282
核心就是说：当我们在计算一个模型在一个平台上的理论性能时，我们不仅要关注平台的性能指标。还要关注模型（也就是算法）的性能指标，这两者之间的关系将决定我们的模型能否达到平台的理论性能上限

> 其实 Roof-line Model 说的是很简单的一件事：**模型在一个计算平台的限制下，到底能达到多快的浮点计算速度**。更具体的来说，Roof-line Model 解决的，是“**计算量为A且访存量为B的模型在算力为C且带宽为D的计算平台所能达到的理论性能上限E是多少**”这个问题。

因此对于一个机器学习模型，当我们使其在计算平台上跑时，我们可以估计该模型的性能能否全部发挥出来

Next, we give some parameters to characterize the roofline model:
ridge point: the point at which the memory and computation bounds coincide. **characterizes the overall machine performance**

- The more the ridge point is “to the left”, the relatively easier it is to get peak performance

**Nominal performace:** the lecturer gives a principle to estimate nominal $\pi$ and $\beta$.
![[../Photo/hpcse/Pasted image 20221005165436.png]]
似乎和处理器的时钟频率有关

Then, we found that the real measured performance have a discrepancy from nominal performance. This may reveal 

- Programming skills in extracting system performance
- best case scenario for more complex kernels

## 1.3 Cache/Thread-Level Parallelism

**Outline:**

- Cache Hierarchy and logic
- cache optimization


- threads: [线程](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B/103101?fr=kg_general)   process：[进程](https://baike.baidu.com/item/%E8%BF%9B%E7%A8%8B/382503?fromModule=lemma-qiyi_sense-lemma)
  - 进程是一个具有一定独立功能的程序关于某个数据集合的一次运行活动。它是操作系统动态执行的基本单元，在传统的操作系统中，进程既是基本的分配单元，也是基本的执行单元。
  - 线程（英语：thread）是操作系统能够进行运算调度的最小单位。它被包含在进程之中，是进程中的实际运作单位。一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。
- concurrency（并发性）；parallelism
- registers：寄存器
- memeory latency [内存延迟](https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E5%BB%B6%E8%BF%9F/6052879?fr=aladdin) 本质原因是处理器和内存的频率不同
- CPU频率：就是CPU的时钟频率，简单说是CPU运算时的工作频率(1秒内发生的同步脉冲数)的简称。单位是Hz。它决定计算机的运行速度，随着计算机的发展，[主频](https://product.pconline.com.cn/itbk/diy/cpu/1110/2565729.html)由过去MHZ发展到了现在的GHZ(1G=1024M)。通常来讲，在同系列[微处理器](https://product.pconline.com.cn/itbk/diy/cpu/1108/2513335.html)，主频越高就代表计算机的速度也越快，但对与不同类型的处理器，它就只能作为一个参数来作参考。
- 内存：[RAM; ROM; Cache三者之间的关系](https://wenku.baidu.com/view/38c7177d757f5acfa1c7aa00b52acfc788eb9f55.html)
  - 通常所说的内存是指的随机存储器。只读存储器通常只能用来读取，通常是系统，如bios这些装在上面。cache位于内存和处理器之间。

- 1 byte = 8 bits [1] 
  1KiB= 1,024 bytes [1] 
  1MiB= 1,048,576 bytes [1] 
  1GiB= 1,073,741,824 bytes [1] 
  1TiB= 1,099,511,627,776 bytes [1] 

- register（寄存器），cache（缓存）与内存（ram）之间的联系【[参考](https://www.eefocus.com/e/502604)】：按与CPU远近来分，离得最近的是寄存器，然后缓存，最后内存。寄存器是最贴近CPU的，而且CPU只与寄存器中进行存取。
  - 寄存器是CPU内部用来存放数据的一些小型存储区域，用来暂时存放参与运算的数据和运算结果。其实寄存器就是一种常用的时序逻辑电路，但这种时序逻辑电路只包含存储电路。寄存器的存储电路是由锁存器或触发器构成的，因为一个锁存器或触发器能存储1位二进制数，所以由N个锁存器或触发器可以构成N位寄存器。寄存器是中央处理器内的组成部分。寄存器是有限存储容量的高速存储部件，它们可用来暂存指令、数据和位址。	
- HDD (hard disk drive): 机械硬盘
- 缓存在CPU上，是CPU的组成成分之一
- difference between concurrency and parallelism: https://blog.csdn.net/weixin_47513022/article/details/115656874

### Lecture

1. Cache Hierarchy: a brief history


2. How does a Cache work
   42:15

## 1.4 Thread-Level Parallelism

- concurrency & parallelism
- threading models and libraries
- Intro to OpenMP

- threads: [线程](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B/103101?fr=kg_general)   process：[进程](https://baike.baidu.com/item/%E8%BF%9B%E7%A8%8B/382503?fromModule=lemma-qiyi_sense-lemma)
  - 进程是一个具有一定独立功能的程序关于某个数据集合的一次运行活动。它是操作系统动态执行的基本单元，在传统的操作系统中，进程既是基本的分配单元，也是基本的执行单元。
  - 线程（英语：thread）是操作系统能够进行运算调度的最小单位。它被包含在进程之中，是进程中的实际运作单位。一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。
- concurrency（并发性）；parallelism

<img src="assets/image-20230106165634768.png" alt="image-20230106165634768" style="zoom:50%;" />

**Introduction:** 

- 首先介绍了concurrency和parallelism的概念，接着进一步介绍实现parallelism的几种方式。其中包含非常重要的Thread-level parallelism。作者介绍了几种threading models和相应的库。最后着重介绍了OpenMP。这里主要是说明如何通过OpemMP来实现并行。

- 我觉得我们的核心是要从硬件和软件两个角度去认识并行

- [API是什么](https://www.zhihu.com/question/38594466/answer/487039892)

  - MPI, Pthreads, OpenMP and CUDA, four our the most widely used APIs for parallel programming.

- 进程和线程 [参考内容](https://tanqingbo.cn/OS-processes-threads/)

  - 进程是程序执行的最小单元，而线程是操作系统分配资源的最小单元

  - 一个进程由多个线程组成。线程是一个进程中代码的不同执行路线。

  - 当我们打开一个应用程序的时候，系统便会为这个应用程序划分相应的进程：**进程**是一个具有一定独立功能的程序在一个数据集上的一次动态执行的过程，是==操作系统进行资源分配和调度的一个独立单位==

  - 进程一般由程序、数据集合和进程控制块三部分组成。

    - 程序用于描述进程要完成的功能，是控制进程执行的指令集；
    - 数据集合是程序在执行时所需要的数据和工作区；
    - 程序控制块(Program Control Block，简称PCB)，包含进程的描述信息和控制信息，是进程存在的唯一标志。	

  - **线程**是程序执行中一个单一的顺序控制流程，是程序执行流的最小单元，是==处理器调度和分派的基本单位（正因为其实处理器调度和分配的基本单元，因此我们才会想到合理分配线程，从而达到高效并行计算）==。一个进程可以有一个或多个线程，各个线程之间共享程序的内存空间。

  - 线程组成：

    - 线程ID
    - 当前指令指针(PC)
    - 寄存器和堆栈。
    - **而进程由内存空间(代码、数据、进程空间、打开的文件)和一个或多个线程组成。**

  - <img src="assets/image-20230106165728533.png" alt="image-20230106165728533" style="zoom:40%;" />

    进程和线程之间的关系

  - <img src="assets/image-20230106165818695.png" alt="image-20230106165818695" style="zoom:40%;" />

  - 

# 2. HPC Libraries

## 2.1 OpenMP

### 2.1.1 Basics

$\bbox[lightgreen]{Concurrency\ and\ Parallelism}$ 

**Concurrency:** The existence of two or more stream of instructions, whose execution order cannot be determined a priori.

**Parallelism:** The existence of two or more stream of instructions executing simultaneously.

<font color=red> There can be concurrency without parallelism, but there cannot be parallelism without concurrency. </font>

$\bbox[lightgreen]{Support\ for\ Parallelism\ in\ Hardware}$

-  Multiple physical cores (Thread-Level Parallelism)
- Pipelining (Instruction-Level Parallelism)
- Vectorization (Data-Level Parallelism)

<img src="assets/image-20230106170640201.png" alt="image-20230106170640201" style="zoom:42%;" /><img src="assets/image-20230106170701334.png" alt="image-20230106170701334" style="zoom:50%;" />

<img src="assets/image-20230106170824205.png" alt="image-20230106170824205" style="zoom:50%;" />



$\bbox[lightgreen]{Processes\ and\ Threads}$

**Processes:**

- Are OS structures (memory+code+threads)
- Operate on their own private virtual address space (64-bit addresses)
- Managed by the OS scheduler
- Contain one or more threads

**Threads (Kernel-Level):**

- Represents a CPU execution state (register values, program counter)
- Executes a stream of instructions (code) within a running process
- All threads associated to the same process share the same virtual address space
- Programmer can control creation/deletion of additional threads



### 2.2.2 Threading Libraries

$\bbox[lightgreen]{POSIX\ Threads\ (Pthreads)}$

- Standardized C language threads programming interface http://pubs.opengroup.org/onlinepubs/9699919799/
- Header file: `#include <pthread.h>`
- Compilation: `gcc -pthread -o myprog myprog.c`
- Allows access to low-level features on shared memory platforms
- More lines of code compared to higher-level API’s
- Not straightforward for incremental parallelism

