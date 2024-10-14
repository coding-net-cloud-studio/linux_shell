#include <unistd.h>    // 包含unistd.h头文件,提供对POSIX操作系统API的访问
#include <stdlib.h>    // 包含stdlib.h头文件,提供通用工具函数
#include <fcntl.h>     // 包含fcntl.h头文件,提供文件控制操作
#include <syslog.h>    // 包含syslog.h头文件,提供系统日志功能
#include <string.h>    // 包含string.h头文件,提供字符串处理函数
#include <errno.h>     // 包含errno.h头文件,定义了错误码
#include <stdio.h>     // 包含stdio.h头文件,提供标准输入输出功能
#include <sys/stat.h>  // 包含sys/stat.h头文件,提供文件状态信息

// 定义锁文件的路径,通常位于 /var/run 目录下,用于存储守护进程的 PID
#define LOCKFILE "/var/run/daemon.pid"

// 定义锁文件的权限模式,包括用户读,写权限,组读权限和其他读权限
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// 声明外部函数 lockfile,该函数用于锁定文件,防止多个进程同时访问
extern int lockfile(int);

/**
 * 检查程序是否已经在运行.
 *
 * 这个函数通过尝试创建一个锁文件来检查程序是否已经在运行.
 * 如果锁文件已经存在,并且当前进程无法获取锁,则认为程序已经在运行.
 *
 * @return 如果程序已经在运行,返回 1;否则返回 0.
 */
int already_running(void)
{
    int  fd;
    char buf[16];

    // 打开或创建锁文件,设置读写权限和锁模式
    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0)
    {
        // 记录错误信息到系统日志
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        // 退出程序
        exit(1);
    }
    // 尝试获取锁文件的独占锁
    if (lockfile(fd) < 0)
    {
        // 如果获取锁失败,并且错误码是 EACCES 或 EAGAIN,表示文件被占用
        if (errno == EACCES || errno == EAGAIN)
        {
            // 关闭文件描述符
            close(fd);
            // 返回 1,表示程序已经在运行
            return (1);
        }
        // 记录错误信息到系统日志
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        // 退出程序
        exit(1);
    }
    // 将文件截断为 0,表示清空文件内容
    ftruncate(fd, 0);
    // 将当前进程的 PID 写入锁文件
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    // 返回 0,表示程序没有在运行
    return (0);
}
