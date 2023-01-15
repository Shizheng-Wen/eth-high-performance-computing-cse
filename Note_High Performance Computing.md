# 1. Theoretical Foundation

## Introduction

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

## High performance computing (HPC) principles, metrics, and models

**Outline: **

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

## Cache/Thread-Level Parallelism

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

## Thread-Level Parallelism

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

  - <img src="assets/image-20230106165728533.png" alt="image-20230106165728533" style="zoom:15%;" />

    进程和线程之间的关系

  - <img src="assets/image-20230106165818695.png" alt="image-20230106165818695" style="zoom:20%;" />

___

# 2. HPC Libraries

## OpenMP

### Basics

$\bbox[lightgreen]{\text{Concurrency and Parallelism}}$ 

**Concurrency:** The existence of two or more stream of instructions, whose execution order cannot be determined a priori.

**Parallelism:** The existence of two or more stream of instructions executing simultaneously.

<font color=red> There can be concurrency without parallelism, but there cannot be parallelism without concurrency. </font>

$\bbox[lightgreen]{\text{Support for Parallelism in Hardware}}$

-  Multiple physical cores (Thread-Level Parallelism)
- Pipelining (Instruction-Level Parallelism)
- Vectorization (Data-Level Parallelism)

<img src="assets/image-20230106170640201.png" alt="image-20230106170640201" style="zoom:30%;" /><img src="assets/image-20230106170701334.png" alt="image-20230106170701334" style="zoom:30%;" />

<img src="assets/image-20230106170824205.png" alt="image-20230106170824205" style="zoom:35%;" />

$\bbox[lightgreen]{\text{Processes and Threads}}$

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

### Threading Libraries

$\bbox[lightgreen]{\text{POSIX Threads (Pthreads)}}$

- Standardized ==C language== threads programming interface http://pubs.opengroup.org/onlinepubs/9699919799/
- Header file: `#include <pthread.h>`
- Compilation: `gcc -pthread -o myprog myprog.c`
- Allows access to low-level features on shared memory platforms
- More lines of code compared to higher-level API’s
- Not straightforward for incremental parallelism

$\bbox[lightgreen]{\text{C++11 Threads}}$

- POSIX threads API is quite rigid and requires lots of additional code
- Another possibility to work with threads is available in C++ (2011 standard) 
- Integrate well with ==C++== and its features
- Relatively easy to get started with
- Header file:`#include <thread>`
- Compilation: `g++ -std=c++11 -pthread -o my-dog my-dog.cpp`

**Comparision between two Threads-oriented libraries:**

<img src="assets/image-20230111145248965.png" alt="image-20230111145248965" style="zoom:30%;" />

$\bbox[lightgreen]{\text{OpenMP: Open Multi-Processing}}$

An **A**pplication **P**rogramming **I**nterface (API) to explicitly define mutli-threaded parallelism on shared memory architectures.

- **Comprised of 3 components:**
  - Pre-compiler directives (#pragma) (not present in POSIX or C++11)
  - Runtime library calls [Optional]
  - Environment variables (not present in POSIX or C++11) [Optional]
- **Header file:** `#include <omp.h>` **[Optional]**
- **Compile with:**
  - `g++ -fopenmp -o my-dog my-dog.cpp` (GCC)
  - `clange++ -fopenmp -o my-dog my-dog.cpp` (LLVM)
  - `icpc -qopenmp -o myprog myprog.cpp` (Intel)

$\bbox[lightgreen]{\text{Reason why we use OpenMP}}$

**Pros:**

- Easy to use and allows to parallelize existing serial programs with minimum effort 
- Parallelization can be done incrementally
- Widely used and supported
- Portability:
  - C/C++ and Fortran
  - Supports Unix/Linux platforms as well as Windows

**Cons:**

- Does not automatically check for race conditions, deadlocks
- Is not designed for distributed memory systems
- Nor is it designed for parallel I/O (user is responsible for synchronization)

**Comparision between two Threads-oriented libraries:**

<img src="assets/image-20230111154426056.png" alt="image-20230111154426056" style="zoom:30%;" />

### Syntax

$\bbox[lightgreen]{\text{Usage of OpenMP}}$

**OpenMP Syntax Format:**

- Precompiler directives:
  - c/c++: `#pragma omp construct [clause [clause] ...]`
- since we use directives, no changes need to be made for a complier that doesn't support OpenMP!

**Thread Count:**

The number of threads in a parallel region is determined by the following factors, in order of precedence:

1. Evaluation of an `if` clause within the omp pragma
2. Setting the `num_threads` clause  eg:`#pragma omp parallel num_threads(2)`
3. Use of the `omp_set_num_threads()` library function
4. Setting the `OMP_NUM_THREADS` environment variable
5. Implementation default — usually the number of CPUs available

**Core Concepts:** Fork/Join Parallel Regions

- Program executes in serial until it encounters a parallel directive
- The thread that encounters the parallel region creates the children (team of threads)
- The encountering thread becomes the master thread inside the region (thread_id = 0)
- The threads synchronize at a barrier at the end of the parallel region. The master thread then continues

```c++
#pragma omp parallel [clause llist] new-line
​      Structured block
```

**Example:**

<img src="assets/image-20230112153940827.png" alt="image-20230112153940827" style="zoom:30%;" />

- Note: If the structure block in a single line, then you may omit the curly braces.

Output: 

```c++
1 Hello from thread Hello from thread Hello from thread Hello from thread 3 2 201
2 201
3
4
```

#### Work Sharing Constructs

$\bbox[lightgreen]{\text{Definition of Work Sharing Constructs}}$

OpenMP gives us access to the following work sharing constructs:

- *Loop construct*: Specifies that the iterations of one or more associated loops will be executed in parallel by the threads in the team. ==Used very often!==
-  *Sections construct*: A non-iterative work sharing construct that contains a set of structured blocks that are to be distributed among and executed by the threads in a team.
- *Single construct*: Specifies that the associated structured block is executed by only one of the threads in the team (not necessarily the master thread).

<font color=red> **Remember:**</font> All work sharing constructs have an implicit barrier at the end (unless the *nowait* clause is used).

**Example:** Parallelizing a For-loop Manually

<img src="assets/image-20230112155639543.png" alt="image-20230112155639543" style="zoom:30%;" />

- Manually allocating the the number of points needed to operate in every thread is very cumbersome, but is very often in MPI. If we use `#pragma omp parallel`, OpenMP does the partitioning of work for us! (Less code, cleaner and more readable!)

==Next, we will discuss these sharing construct one by one!==

##### Loop Construct (Parallel for-loops)

$\bbox[lightgreen]{\text{Definition}}$

The loop construct is the most important of the OpenMP work sharing constructs. Let’s look a bit closer:

```c++
#pragma omp for [clause[[,]clause]...] new-line
for-loops
```

**Clause can be:**

- `private(list)`; `firstprivate(list)`; `lastprivate(list)`
- `schedule(kind [, chunk_size])`
- `collapse(n)`
- `reduction(reduction-identifier:list)`
- `nowait`
- Note: There are a few more clauses that we do not discuss in this lecture notes. You can find a full list in the OpenMP specification. https://www.openmp.org/specifications/

$\bbox[lightgreen]{\text{regular use}}$

- `#pragma omp parallel`
  - 后面的structure block被并行执行，设置了多少个thread，等价上来说就是执行多少次
  - 相当于并行的thread做了同一个事情，代码执行只有速度上的差异，没有执行量上的差异。
  - <img src="assets/image-20221106172005068.png" alt="image-20221106172005068" style="zoom:20%;" />

- `#pragma omp parallel for`

  - 后面的for循环被拆分成多少个子快，每个子块通过一个thread去并行。

  - 但是这里需要注意的是，如果各个子块thread之间有数据依赖的话，则很有可能会出错
  - <img src="assets/image-20221106171854721.png" alt="image-20221106171854721" style="zoom:22%;" />
  - <font color=red> 并行潜力的实现主要体现在对循环的处理上.</font>

$\bbox[lightgreen]{\text{Loop Scheduling}}$

**Motivation:** There are different ways to distribute the work among threads in a team. We care about this to have more control over parallel granularity and load-balancing.

<font color=red> Usually, we discussed about loop scheduling in `for` loop. </font> 

```c++
#pragma omp for schedule (kind [, chunk_size]) new-line
for-loops
```

- <img src="assets/image-20221106184908894.png" alt="image-20221106184908894" style="zoom:15%;" />
  - 内存的访问是连续的，在读入cache中，不进行大量换入换出等额外开销。
- <img src="assets/image-20221106185057905.png" alt="image-20221106185057905" style="zoom:15%;" />

**Solutions: 循环调度(Loop Scheduling):**

循环调度的种类：

- 静态调度：在循环执行之前，就已经将循环任务分配好 （就像我们的`#pragma omp parallel for`）
- 动态调度：在循环执行的过程中，边执行变分配

OpenMP提供了一个选项`schedule`，它能够将循环的分配给每个线程。当采用不同的参数的时候，我们会使用不同的调度方式。

- `schedule(static[,chunk])`
  - 当采用static的参数时，chunk代表了每一块分块的大小
  - 他会采取轮转制度，按照线程的数量，获得分配的一块大小的内容。在循环开始前就已经确定。
  - 低开销，但是可能会造成分配不均
  - <img src="assets/image-20221106185701294.png" alt="image-20221106185701294" style="zoom:20%;" />

- `schedule(dynamic[,chunk])`

  - 当采用dynamic的参数时

  - chunk代表了每一块分块的大小

  - 每个线程执行完毕后，会自动获取下一块

  - 高开销，但是能减少分配不均衡的情况

  - <img src="assets/image-20221106185923258.png" alt="image-20221106185923258" style="zoom:20%;" />

    

    

- `schedule(guided[,chunk])`

  - 当采用guide的参数时，会按照一定的规则分配块
  - 这是一种动态的分配
  - 每一块的分配的数量时在不断收缩的，但是最小不会小于chunk（默认是1)
  - 最初的块会被定义成：
    - 循环数量/线程数
  - 其余块的大小会被定义成：
      - 剩余循环数量/线程数
  - 计算效率比dynamic高一点点。且也有缺陷，例如这个循环从下往上执行，就会导致第一个thread承担非常大的负担。
  - <img src="assets/image-20221106190147503.png" alt="image-20221106190147503" style="zoom:20%;" />

**Loop scheduling kind (in a picture):**

<img src="assets/image-20230113141418479.png" alt="image-20230113141418479" style="zoom:30%;" />

$\bbox[lightgreen]{\text{Nested Loops}}$

**Motivation:** How does OpenMP parallelize nested loops:

```c++
#pragma omp parallel
{
  #pragma omp for
  for(int i=0;i<N;++i){
    for(int j=0;j<N;++j){
      A[i][j]= //do something here
    }
  }
}
```

- The `#pragma omp for` applies to the for-loop that immediately follows it
- Because the innermost loop and the outermost loop are independent, can we use more efficient way for parallel?

**Solution: Loop Collapsing**

Multiple loops associated to a loop construct can be collapsed (combined or fused) into a single loop with a larger iteration space if they are ==perfectly nested==.

```c++
#pragma omp for collapse(n) new-line
for-loops
```

- `n` must be a constant positive integer expression.
- <font color=red>But be aware:</font> If execution of any associated loop changes any of the values used to compute any of the iteration counts, then the behavior is unspecified!
- Benefits of loop collapsing: (1) Can achieve better utilization of available resources; (2) Increased performance due to better load-balancing.

**Example 1:**

<img src="assets/image-20230113142744079.png" alt="image-20230113142744079" style="zoom:30%;" />

**Example 2:**

<img src="assets/image-20230113142931913.png" alt="image-20230113142931913" style="zoom:26%;" />

$\bbox[lightgreen]{\text{Reduction}}$

**Definition:** A reduction is a common operation where a specific operation is performed on all elements on some set of data (==distributed on different threads==). This can be done efficiently with the reduction clause:

<img src="assets/image-20230113143307744.png" alt="image-20230113143307744" style="zoom:25%;" />

**Example:** Sum the elements in an array in parallel

<img src="assets/image-20230113143545527.png" alt="image-20230113143545527" style="zoom:30%;" />

**Detailed Introduction of Reduction:(归并）**

reduction也是一种常见的选项，它为我们的parallel，for和sections提供一个归并的功能。【在Eigen中，我们也常常遇到数据reduction的情况，在这种情况our motivation往往是将一个Vector/Matrix归并成一个single scalar】

使用方法如下：

`#pragma omp ... Reduction(op:list)`

他会提供一个私有的变量拷贝并且初始化该私有变量。

私有变量的初始化的值取决于选择的归并的操作符。

这些变量的拷贝会在本地线程进行更新。

在最后的出口中，所有的变量拷贝将会通过操作符所定义的规则进行合并的计算，计算成一个共享变量【reduction的核心部分】。

- 有点像`lastprivate`选项，但`lastprivate`只会最后用最后一个线程的私有变量对公有变量进行赋值。而reduction则是按照运算符讲所有线程中操作后再对公有变量进行运算和赋值（初始变量原有的值也会在操作中，这点不像`lastprivate`会选择直接覆盖。
- <img src="assets/image-20221106174435764.png" alt="image-20221106174435764" style="zoom:15%;" />



##### Section Construct

`sections` can be used to assign individual structured blocks to different threads in a team:

<img src="assets/image-20230112161625942.png" alt="image-20230112161625942" style="zoom:30%;" />

**sections 和section：**

我们希望不同的线程执行不一样的代码

- `section`
  - sections在封闭代码的指定部分中，由线程组进行分配任务。当然我们也可以用`tid==1，tid==0`来确定执行

  - 每个独立的section都需要在sections里面。

    - 每个section都是被一个线程执行的。

    - 不同的section可能执行不同的任务

    - 如果一个线程够快，该线程可能执行多个section 【因为使用线程组分配的】



##### Single Construct

`single` can be used to assign a structured block to one single thread in a team (can be considered as a synchronization construct as well):

<img src="assets/image-20230112161735820.png" alt="image-20230112161735820" style="zoom:30%;" />

- `single` ：该选项是在并行块里使用的
  - 它告诉编译器接下来紧跟的下段代码将会由只一个线程执行。
  - 它可能在处理多段线程不安全代码时非常有用
  - 在不使用no wait选项时，在线程组中不执行single的线程们将会等待single的结束
- `master`：该选项是在并行块中使用的
  - 编号为0的线程master进行执行
  - 它告诉编译器接下来的一段代码只有有主线程执行
  - 它不会出现等待现象 <font color=red> 所谓是否会出现等待造成的影响就是共有数据是否会出现混乱访问。</font>
  - <img src="assets/image-20221106173148155.png" alt="image-20221106173148155" style="zoom:20%;" />

##### Combined Construct

**Motivation:** There is only a single *for*-loop in the parallel region, do I need all this code?

**Solution:** No, combined constructs are shortcuts for specifying one construct immediately nested inside another construct. This applies to the `loop` and `sections` constructs we have seen so far.

**Example:**

<img src="assets/image-20230113143952482.png" alt="image-20230113143952482" style="zoom:27%;" />

**<font color=red>Common beginners mistake</font>** is as follows:

<img src="assets/image-20230113144103319.png" alt="image-20230113144103319" style="zoom:27%;" />

##### Synchronization Constructs

同步构造 (synchronization constructs) 确保对线程组之间**共享的内存地址**的**一致访问**，用于**顺序约束**和共享数据的**访问保护**。OpenMP supports several synchronization constructs:

- `critical` (critical sections)
- `atomic`
- `barrier`
- `section` `master` (in fact, not a synchronization construction)
- `ordered` `flush` `lock` (not discussed in the lecture)

==Synchronization constructs ensure consistent access to memory address that is shared among a team of threads.==

$\bbox[lightgreen]{\text{Race Conditions}}$

Let's use an simple example to explain the race conditions

​                        <img src="assets/image-20230113155740367.png" alt="image-20230113155740367" style="zoom:28%;" /><img src="assets/image-20230113155759456.png" alt="image-20230113155759456" style="zoom:32%;" />

Output of the correspond code: <img src="assets/image-20230113155906122.png" alt="image-20230113155906122" style="zoom:25%;" />

From the above example, we can notice that the code for multi-threads is influenced by race conditions:

<img src="assets/image-20230113160121020.png" alt="image-20230113160121020" style="zoom:25%;" />

- At one time, the shared memory is accessed by multiple threads. Furthermore, we will define the criticial sections as follows:

$\bbox[lightgreen]{\text{Critical Sections}}$

<img src="assets/image-20230113155531872.png" alt="image-20230113155531872" style="zoom:30%;" />

==We need to limit threads to access critical sections one at a time: We use ==Mutual Exclusion Mechanisms==.

#### Data Environment

$\bbox[lightgreen]{\text{Introduction}}$

**Recall:** We are working in a shared memory environment.

- In OpenMP, variables outside a parallel region are shared by default.
- Global/static variables are shared.
- Not everything is shared in OpenMP.
  - Loop index variables
  - Automatic variables in functions that are called inside the parallel region.

**Three types of data-sharing attributes in OpenMP:**

1. Predetermined (eg. Loop counters, `threadprivate` variables)
2. Explicitly determined (what you specify in a clause of a construt)
3. Implicitly determined (everything else)

<font color=purple>**Note:**</font> Data-sharing clauses can only be used in the <font color=purple>lexical extent</font> of the OpenMP directive.





Commonly used cluase:

- `default(shared | none)`
- `shared(list)`
- `private(list)`
- `firstprivate(list)`
- `lastprivate(list)`

**Default Variable 默认变量:**

- `pragma omp ...default (shared | none)
  - You can chang the default (shared, implicitly determined). The clause must only appear once
  - `none` means that every variable you reference in a parallel region must be explicitly determined (unless its attribute is predetermined)
- <img src="assets/image-20230113150539545.png" alt="image-20230113150539545" style="zoom:30%;" />

**Private variable 私有变量：**

- `#pragma omp ... private(<variable list>)` 
  - 变量作用域分为私有变量和公有变量。上面的命令能够直接告诉编译器去使得共享变量作为每个线程中的私有变量。 Creates a ==private copy== of each variable in the list. The private copy has a different address ==(avoiding data races)==
  - 如果j被定义为私有变量，那么在for循环里面，所有的线程都不能访问其他j（尽管j是共享变量）
  - 所有线程都不会使用到先前的定义。也就是私有变量在各个thread中会被初始化，和之前的值完全没有关系 The private copy is not initialized, no association with the global variable.
  - 所有线程都不能给共享的j赋值
  - 在循环的入口以及出口，都不会进行定义（就是前两条的总结）
  - <img src="assets/image-20230113150125031.png" alt="image-20230113150125031" style="zoom:25%;" />
- `#pragma omp parallel for firstprivate(x)`
  - Special case of `private (list)`
  - 告诉编译器私有变量在第一个循环会继承共享变量的值 The private copy is ==initialized== to the value seen by the `master` thread. Its association with the global variable is its ==initialization.==
  - 其使用方法和private几乎一致。
  - 其本质就是在第一个循环实现了数据的一个拷贝（从共有 $\rightarrow$ 私有）
  - <img src="assets/image-20230113150800598.png" alt="image-20230113150800598" style="zoom:30%;" />
- `#pragma omp parallel for lastprivate(x)`
  - Special case of `private (list)`
  - The private copy is not initialized, as in `private(list)`
  - Its association with the global variable is its update after the parallel region (<font color=red>careful:</font> data races) 告诉编译器私有变量会在最后一个循环出去的时候，用私有变量的值替换掉我们的共享变量的值。
  - 因为不同thread的速度不一样，到底是哪个thread进行赋值呢？答：当负责最后一个iteration的线程离开循环的时候，它会将该私有变量的值赋值给当前共享变量的值。
  - <img src="assets/image-20230113151112578.png" alt="image-20230113151112578" style="zoom:30%;" />



**OpenMP同步与互斥：**

在很多时候，需要线程之间团结协作完成某个任务，这就要求线程能够完成一致协调合作。

OpenMP提供了多个操作。

Barriers、critical分别用于实现同步与互斥。

- `barrier` 实现不同thread之间的同步

  - 用与实现同步的一种手段。他会在代码的某个点，令线程停下直到所有的线程都到达该地方。
  - 使用的语法如下：
  - `#pragma omp barrier`
  - 许多情况下，它已经能够自动的插入到工作区结尾。比如说在for，single。但是它能够被nowait禁用。
  - <img src="assets/image-20221106175615574.png" alt="image-20221106175615574" style="zoom:20%;" />

- `nowait`

  - 用于打断自动添加的barrier的类型

  - 如parallel中的for以及single
    - `#pragma omp for nowait`
    - `#pragma omp single nowait`

**OpenMP竞争：**  

- `#pragma omp critical`

  - OpenMP提供了一个实现互斥的接口。critical选项
  - 使用方法 `#pragma omp critical`
    - 它告诉编译器在接下来的一段代码在同一个时间段将会只由一个线程进行
    - 如果一段代码在同一个时间段被多个线程进行，那么有可能在数据的写入或者读出时，会发生竞争。
    
  - 好处：解决了竞争现象
  - 坏处：使用critical会让程序执行减少并行化程度。必须要写代码的人手动判断哪些部分需要critical 【保护某些关键数据，一般来说就是修改我们的共享变量时候】 

- `#pragma omp atomic`

  - 在特殊的情况下，除了使用critical选项控制临界区以外，我们还可以使用其他选项去保证内存的控制是原子的

  - OpenMP提供了一个选项：atomic（原子）

  - 它只在特殊的情况下使用：

    - 在自增或者自减的情况下使用
    - 在二元操作数的情况下使用

  - 只会应用于一条指令

    - ```c++
      #pragma omp atomic
      counter +=5;
      ```

    - <img src="assets/image-20221106184757451.png" alt="image-20221106184757451" style="zoom:20%;" />

- - -

### Common Usage

1.  `#pragma omp parallel num_threads(i) if (paralle: i>2)`: Explicitly specify to which directive the if applies. Useful for composite and combined constructs. If not specified, the if applies to all constructs to which an if clause can apply.

   ```c++
   for (int i = 1; i <= 4; ++i){
   cout<<"Number of threads ="<<i<<endl;
   #pragma omp parallel num_threads(i) if (paralle: i>2)
     {
       const int tid=omp_get_thread_num();
       #pragma omp critical//the next line is a critical section!
       cout<<"Hello from the thread"<<tid<<'\n';
     }}
   ```

   
