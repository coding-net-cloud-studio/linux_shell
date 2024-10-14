// 包含APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include "apue.h"

// 包含errno头文件,用于访问系统调用的错误代码
#include <errno.h>

// 包含fcntl头文件,提供了文件控制操作,如文件锁定,记录锁定等
#include <fcntl.h>

// 包含sys/wait.h头文件,提供了进程等待和终止的相关函数
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int         fd;       // 文件描述符
    pid_t       pid;      // 进程ID
    char        buf[5];   // 缓冲区,用于存储读取的数据
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

    // 设置文件的set-group-ID位,并清除group-execute位

    // 使用fstat函数获取文件的状态信息,如果失败则调用err_sys函数报告错误
    if (fstat(fd, &statbuf) < 0)
        err_sys("fstat error");

    // 使用fchmod函数修改文件的权限,移除组执行权限并设置SGID位,如果失败则调用err_sys函数报告错误
    // statbuf.st_mode & ~S_IXGRP: 移除组执行权限
    // S_ISGID: 设置SGID位
    if (fchmod(fd, (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err_sys("fchmod error");

    TELL_WAIT();  // 通知子进程等待

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid > 0)
    {  // 父进程
        // 对整个文件加写锁
        if (write_lock(fd, 0, SEEK_SET, 0) < 0)
            err_sys("write_lock error");

        TELL_CHILD(pid);  // 通知子进程可以继续

        // 等待子进程结束
        if (waitpid(pid, NULL, 0) < 0)
            err_sys("waitpid error");
    }
    else
    {                   // 子进程
        WAIT_PARENT();  // 等待父进程加锁

        set_fl(fd, O_NONBLOCK);  // 设置文件描述符为非阻塞模式

        // 尝试对已加锁的区域加读锁,预期失败
        // 尝试对已锁定的区域进行读锁定,不等待锁定释放
        // 如果锁定成功(函数返回值不为-1),则打印错误信息
        if (read_lock(fd, 0, SEEK_SET, 0) != -1)  // 不等待
            err_sys("child: read_lock succeeded");

        // 打印尝试锁定已锁定区域时的errno值,以查看操作系统的错误代码
        printf("read_lock of already-locked region returns %d\n",
               errno);

        // 尝试读取强制锁定的文件,预期失败
        // 尝试将文件指针移动到文件的开始位置
        if (lseek(fd, 0, SEEK_SET) == -1)
            // 如果lseek失败,打印错误信息并退出
            err_sys("lseek error");
        // 尝试从文件中读取2个字节的数据到buf
        if (read(fd, buf, 2) < 0)
            // 如果read失败,打印错误信息
            err_ret("read failed (mandatory locking works)");
        else
            // 如果read成功,打印读取的数据
            printf("read OK (no mandatory locking), buf = %2.2s\n",
                   buf);
    }
    exit(0);
}
