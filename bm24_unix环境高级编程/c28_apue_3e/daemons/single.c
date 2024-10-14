#include <unistd.h>    // 包含unistd.h头文件,提供对POSIX操作系统API的访问
#include <stdlib.h>    // 包含stdlib.h头文件,提供内存管理,随机数生成等功能
#include <fcntl.h>     // 包含fcntl.h头文件,提供文件控制功能
#include <syslog.h>    // 包含syslog.h头文件,提供系统日志功能
#include <string.h>    // 包含string.h头文件,提供字符串处理功能
#include <errno.h>     // 包含errno.h头文件,提供错误码访问功能
#include <stdio.h>     // 包含stdio.h头文件,提供标准输入输出功能
#include <sys/stat.h>  // 包含sys/stat.h头文件,提供文件状态信息

// 定义锁文件路径
#define LOCKFILE "/var/run/daemon.pid"
// 定义锁文件的权限模式
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// 声明lockfile函数,该函数用于锁定文件
extern int lockfile(int);

/**
 * 检查是否已经有进程在运行
 * @return 如果已经有进程在运行返回1,否则返回0
 */
int already_running(void)
{
    int  fd;       // 文件描述符
    char buf[16];  // 缓冲区,用于存储进程ID

    // 打开或创建锁文件
    fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0)
    {
        // 打开文件失败,记录错误日志并退出程序
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    // 尝试锁定文件
    if (lockfile(fd) < 0)
    {
        // 如果锁定失败,检查错误类型
        if (errno == EACCES || errno == EAGAIN)
        {
            // 文件已经被其他进程锁定,关闭文件描述符并返回1
            close(fd);
            return (1);
        }
        // 其他锁定错误,记录错误日志并退出程序
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    // 截断文件到0长度
    ftruncate(fd, 0);
    // 将当前进程ID写入锁文件
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    // 返回0表示当前进程可以运行
    return (0);
}
