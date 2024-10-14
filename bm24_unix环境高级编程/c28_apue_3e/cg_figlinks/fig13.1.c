// 包含APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口.
#include "apue.h"

// 包含syslog函数的头文件,syslog用于记录系统和应用程序的日志消息.
#include <syslog.h>

// 包含文件控制函数的头文件,如open, read, write等.
#include <fcntl.h>

// 包含资源限制函数的头文件,如getrlimit, setrlimit等.
#include <sys/resource.h>

void daemonize(const char *cmd)
{
    int              i, fd0, fd1, fd2;  // 定义文件描述符和循环变量
    pid_t            pid;               // 定义进程ID
    struct rlimit    rl;                // 定义资源限制结构体
    struct sigaction sa;                // 定义信号动作结构体

    /*
     * 清除文件创建掩码.
     */
    umask(0);

    /*
     * 获取最大文件描述符数量.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: can't get file limit", cmd);  // 获取失败则退出

    /*
     * 成为会话领导者以失去控制TTY.
     */
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);  // 分叉失败则退出
    else if (pid != 0)                    /* 父进程 */
        exit(0);                          // 父进程退出
    setsid();                             // 设置新的会话领导者

    /*
     * 确保未来的打开操作不会分配控制TTY.
     */
    sa.sa_handler = SIG_IGN;   // 忽略SIGHUP信号
    sigemptyset(&sa.sa_mask);  // 清空信号掩码
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can't ignore SIGHUP", cmd);  // 忽略信号失败则退出
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);  // 再次分叉失败则退出
    else if (pid != 0)                    /* 父进程 */
        exit(0);                          // 父进程退出

    /*
     * 将当前工作目录更改为根目录,以便
     * 我们不会阻止文件系统被卸载.
     */
    if (chdir("/") < 0)
        err_quit("%s: can't change directory to /", cmd);  // 更改目录失败则退出

    /*
     * 关闭所有打开的文件描述符.
     */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;  // 如果最大文件数无限,则设置为1024
    for (i = 0; i < rl.rlim_max; i++)
        close(i);  // 关闭所有文件描述符

    /*
     * 将文件描述符0,1和2附加到/dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);  // 打开/dev/null
    fd1 = dup(0);                     // 复制文件描述符0
    fd2 = dup(0);                     // 再次复制文件描述符0

    /*
     * 初始化日志文件.
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);  // 初始化日志系统
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);  // 文件描述符不符合预期
        exit(1);                                                                 // 退出
    }
}
