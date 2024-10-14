#include "apue.h"      // 引入APUE库,提供系统编程接口
#include <fcntl.h>     // 引入文件控制相关的函数定义
#include <sys/mman.h>  // 引入内存映射相关的函数定义

// 定义循环次数和共享内存区域的大小
// 定义循环次数
#define NLOOPS 1000

// 定义long类型的大小,用于计算内存占用
#define SIZE   sizeof(long)

// 更新共享内存中的值,并返回更新前的值
/**
 * @param ptr 指向共享内存区域的指针
 * @return 更新前的值
 */
static int
update(long *ptr)
{
    return ((*ptr)++);  // 返回值在递增之前
}

int main(void)
{
    int   fd, i, counter;  // 文件描述符,循环计数器,计数器值
    pid_t pid;             // 进程ID
    void *area;            // 指向共享内存区域的指针

    // 打开/dev/zero设备文件,用于创建共享内存
    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    // 映射共享内存区域
    // 使用mmap系统调用将文件映射到内存中
    // 参数解释:
    // 0: 映射区域的起始地址,0表示由系统选择
    // SIZE: 映射区域的长度
    // PROT_READ | PROT_WRITE: 映射区域的保护标志,可读可写
    // MAP_SHARED: 映射类型,共享映射,对映射区域的修改会影响到原文件
    // fd: 文件描述符
    // 0: 偏移量,0表示从文件开始处映射
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("mmap error");  // 如果mmap失败,调用err_sys函数输出错误信息

    close(fd);  // 映射完成后关闭文件描述符

    TELL_WAIT();  // 初始化父子进程间的同步

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid > 0)
    {  // 父进程
        for (i = 0; i < NLOOPS; i += 2)
        {
            // 更新共享内存中的值,并检查是否与预期相符
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            TELL_CHILD(pid);  // 通知子进程可以继续执行
            WAIT_CHILD();     // 等待子进程完成
        }
    }
    else
    {  // 子进程
        for (i = 1; i < NLOOPS + 1; i += 2)
        {
            WAIT_PARENT();  // 等待父进程通知

            // 更新共享内存中的值,并检查是否与预期相符
            // 检查从update函数返回的值是否等于i,如果不等则调用err_quit函数报告错误
            // update函数接收一个指向long类型数组的指针,返回一个整数值
            // err_quit函数用于在发生错误时退出程序,并打印错误信息
            if ((counter = update((long *)area)) != i)               // 将update函数的返回值赋给counter变量,并与i进行比较
                err_quit("child: expected %d, got %d", i, counter);  // 如果counter不等于i,打印错误信息并退出程序

            TELL_PARENT(getppid());  // 通知父进程可以继续执行
        }
    }

    exit(0);  // 退出进程
}
