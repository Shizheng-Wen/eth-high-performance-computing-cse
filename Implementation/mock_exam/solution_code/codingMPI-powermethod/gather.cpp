/*----------------------------MPI_Gather-------------------------*/
//功能：
//  Gathers together values from a group of processes
//  将一个进程组的值聚集在一起
//特点：
//  根进程将收到的数据按进程号排序
//  根进程数据接收缓存的大小为该组中每个进程发送缓存大小之和

#include "stdio.h"  
#include "mpi.h"  
#include "stdlib.h"  

int main(int argc,char **argv)  
{  
    int size,rank;  
    static int max=20;  
    int send[10];  
    int *recv;  
    int i=0;
    MPI_Init(&argc,&argv);  
    MPI_Comm_size(MPI_COMM_WORLD,&size);  
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);  

    /*给每个进程的发送区填充数据*/
    for(;i<10;i++)  
        send[i]=i+rank;  
    printf("-----------------------------------------------------------------------\n");
    printf("I'm process %d,and the data of my send buffer is:\n", rank);
    for (i = 0; i < 10; i++)
        printf("%d ", send[i]);
    printf("\n");
    /*以0号进程为根进程并分配所需内存空间[大小为所有进程send数据缓存大小之和]*/
    if(rank==0)  
    {  
        recv=(int *)malloc(size*10*sizeof(int));  
    }

    /*----------------------------MPI_Gather详解-----------------------------*/
    //功能：
    //  Each process (root process included) sends the contents of its send
    //  buffer to the root process. The root process receives the messages and
    //  stores them in rank order.
    //  每个进程(包含根进程)将自己send数据缓存中的数据发送给根进程，根进程接收
    //  所有发来的消并按进程标识排序
    //函数参数：
    //  MPI_Gather(数据发送缓存地址，数据个数，数据类型，数据接收缓存地址，
    //  数据个数，数据类型，根进程标识，通信域）
    MPI_Gather(send,10,MPI_INT,recv,10,MPI_INT,0,MPI_COMM_WORLD);  

    /*结果输出*/
    if(rank==0)  
    {  
        printf("I'm root process,and the data that I received is:\n");
        for(i=0;i<size*10;i++)  
            printf("%d ",recv[i] );  
    }  
    printf("\n");

    MPI_Finalize();  
    return 0;  
}  

