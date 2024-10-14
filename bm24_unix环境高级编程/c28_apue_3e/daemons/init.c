#include "apue.h"          // 引入APUE库头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include <syslog.h>        // 引入系统日志接口,用于记录程序运行时的信息
#include <fcntl.h>         // 引入文件控制选项,用于文件的打开,读写等操作
#include <sys/resource.h>  // 引入资源限制相关的头文件,可以用来设置进程的资源限制

// @brief 将进程转换为守护进程
// @param cmd 进程命令名称,用于日志记录
void daemonize(const char *cmd)
{
    int              i, fd0, fd1, fd2;  // 文件描述符变量
    pid_t            pid;               // 进程ID变量
    struct rlimit    rl;                // 资源限制结构体
    struct sigaction sa;                // 信号动作结构体

    /*
     * 清除文件创建掩码.
     */
    umask(0);

    /*
     * 获取最大文件描述符数.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: can't get file limit", cmd);

    /*
     * 成为会话领导者以失去控制TTY.
     */

    // 使用fork()函数创建一个子进程
    if ((pid = fork()) < 0)
        // 如果fork失败,输出错误信息并退出
        err_quit("%s: can't fork", cmd);
    else if (pid != 0) /* 父进程 */
        // 如果当前进程是父进程,则退出父进程
        exit(0);
    // 创建一个新的会话,使子进程成为新会话的领头进程
    setsid();

    /*
     * 确保未来的打开操作不会分配控制TTY.
     */
    // 忽略SIGHUP信号
    sa.sa_handler = SIG_IGN;                       // 设置信号处理函数为忽略
    sigemptyset(&sa.sa_mask);                      // 清空信号掩码集
    sa.sa_flags = 0;                               // 设置信号处理标志为0
    if (sigaction(SIGHUP, &sa, NULL) < 0)          // 如果设置信号处理失败
        err_quit("%s: can't ignore SIGHUP", cmd);  // 输出错误信息并退出

    // 创建子进程
    if ((pid = fork()) < 0)               // 如果fork失败
        err_quit("%s: can't fork", cmd);  // 输出错误信息并退出
    else if (pid != 0)
    {             /* 父进程 */
        exit(0);  // 父进程退出
    }

    /*
     * 将当前工作目录更改为根目录,以便
     * 我们不会阻止文件系统被卸载.
     */
    if (chdir("/") < 0)
        err_quit("%s: can't change directory to /", cmd);

    /*
     * 关闭所有打开的文件描述符.
     */
    // 限制资源使用,关闭所有文件描述符
    if (rl.rlim_max == RLIM_INFINITY)  // 如果最大资源限制是无穷大
        rl.rlim_max = 1024;            // 则将最大资源限制设置为1024
    for (i = 0; i < rl.rlim_max; i++)  // 遍历从0到最大资源限制的所有文件描述符
        close(i);                      // 关闭每个文件描述符

    /*
     * 将文件描述符0,1和2附加到/dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
     * 初始化日志文件.
     */
    // 打开系统日志,cmd为日志标识,LOG_CONS表示如果日志不能写入系统日志文件,则直接输出到控制台,LOG_DAEMON表示这是守护进程的日志
    openlog(cmd, LOG_CONS, LOG_DAEMON);

    // 检查文件描述符是否正确初始化,标准输入,输出,错误的文件描述符应该分别是0,1,2
    // 如果文件描述符不正确,记录错误日志并退出程序
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}
