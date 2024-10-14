#include "apue.h"      // 引入APUE库的头文件
#include <fcntl.h>     // 引入文件控制相关的头文件
#include <sys/mman.h>  // 引入内存映射相关的头文件

#define NLOOPS 1000          // 定义循环次数
#define SIZE   sizeof(long)  // 定义共享内存区域的大小,这里使用long类型的大小

static int
update(long *ptr)
{
    // 返回指针所指向的值,并在返回之后将值增加1
    // 注意:这个函数返回的是自增前的值
    return ((*ptr)++);
}

int main(void)
{
    int   fd, i, counter;  // 定义文件描述符fd,循环变量i,计数器counter
    pid_t pid;             // 定义进程ID变量pid
    void *area;            // 定义内存映射区域指针area

    // 打开/dev/zero设备文件,获取文件描述符
    if ((fd = open("/dev/zero", O_RDWR)) < 0)
        err_sys("open error");
    // 将/dev/zero映射到内存,获取映射区域指针
    // 如果内存映射失败,将错误信息记录到系统日志
    if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        err_sys("mmap error");  // 记录内存映射错误

    close(fd);  // 映射完成后关闭文件描述符

    TELL_WAIT();  // 通知子进程等待父进程的信号

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 创建子进程失败
    }
    else if (pid > 0)
    { /* parent */  // 父进程代码块
        // 父进程循环更新内存映射区域
        for (i = 0; i < NLOOPS; i += 2)
        {
            // 更新内存映射区域并检查返回值是否正确
            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            TELL_CHILD(pid);  // 通知子进程可以继续执行
            WAIT_CHILD();     // 等待子进程完成
        }
    }
    else
    { /* child */  // 子进程代码块
        // 子进程循环更新内存映射区域
        for (i = 1; i < NLOOPS + 1; i += 2)
        {
            WAIT_PARENT();  // 等待父进程的信号

            // 更新内存映射区域并检查返回值是否正确
            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            TELL_PARENT(getppid());  // 通知父进程可以继续执行
        }
    }

    exit(0);  // 进程退出
}
