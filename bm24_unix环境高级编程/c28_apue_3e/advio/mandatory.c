#include "apue.h"      // 引入APUE库,提供系统编程接口
#include <errno.h>     // 引入错误处理头文件,定义了errno变量和各种错误代码
#include <fcntl.h>     // 引入文件控制操作头文件,如open, fcntl等函数
#include <sys/wait.h>  // 引入进程等待头文件,提供wait和waitpid函数

int main(int argc, char *argv[])
{
    int         fd;       // 文件描述符
    pid_t       pid;      // 进程ID
    char        buf[5];   // 缓冲区
    struct stat statbuf;  // 文件状态结构体

    // 检查命令行参数数量
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }
    // 打开文件,如果不存在则创建,如果存在则截断
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("open error");
    // 写入数据到文件
    if (write(fd, "abcdef", 6) != 6)
        err_sys("write error");

    // 修改文件状态,设置set-group-ID位,清除group-execute位
    if (fstat(fd, &statbuf) < 0)
        err_sys("fstat error");
    if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err_sys("fchmod error");

    TELL_WAIT();  // 通知子进程等待父进程的操作

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid > 0)
    { /* 父进程 */
        // 对整个文件加写锁
        if (write_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("write_lock error");

        TELL_CHILD(pid);  // 通知子进程可以继续

        // 等待子进程结束
        if (waitpid(pid, NULL, 0) < 0)
            err_sys("waitpid error");
    }
    else
    {                   /* 子进程 */
        WAIT_PARENT();  // 等待父进程加锁完成

        set_fl(fd, O_NONBLOCK);  // 设置文件描述符为非阻塞模式

        // 尝试对已加锁的区域加读锁,预期失败
        // mandatory.c

        // 尝试对已锁定的区域进行读锁定
        if (read_lock(fd, 0, SEEK_SET, 0) != -1)
        { /* no wait */
            // 如果成功锁定,打印错误信息,因为不应该成功
            err_sys("child: read_lock succeeded");
        }
        // 打印尝试锁定已锁定区域的返回值,通常是errno
        printf("read_lock of already-locked region returns %d\n", errno);

        // 尝试读取文件,预期因强制锁而失败
        // 以下代码段用于检查文件是否支持强制锁定
        // 如果支持,将无法读取文件的前两个字节;如果不支持,可以成功读取

        // 使用lseek函数将文件指针移动到文件开头
        // 如果失败,调用err_sys函数报告错误
        if (lseek(fd, 0, SEEK_SET) == -1)
            err_sys("lseek error");

        // 尝试读取文件的前两个字节
        // 如果读取的字节数小于0,表示强制锁定生效,调用err_ret函数报告错误
        // 否则,打印读取成功的信息以及读取到的内容
        if (read(fd, buf, 2) < 0)
            err_ret("read failed (mandatory locking works)");
        else
            printf("read OK (no mandatory locking), buf = %2.2s\n",
                   buf);
    }
    exit(0);
}
