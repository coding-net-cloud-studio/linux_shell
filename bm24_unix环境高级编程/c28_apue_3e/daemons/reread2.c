// 包含APUE库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include "apue.h"

// 包含syslog库的头文件,syslog是用于记录系统和应用程序消息的系统服务.
#include <syslog.h>

// 包含errno库的头文件,errno用于在系统调用和某些库函数失败时保存错误码.
#include <errno.h>

// 外部函数声明

// 声明外部函数lockfile,该函数用于锁定文件,防止多个进程同时访问
// 参数:int类型,表示要锁定的文件描述符
// 返回值:int类型,成功锁定返回0,失败返回错误码
extern int lockfile(int);

// 声明外部函数already_running,该函数用于检查是否有同名进程已经在运行
// 返回值:int类型,如果已经有同名进程在运行返回非0值,否则返回0
extern int already_running(void);

// 重新读取配置文件的函数
void reread(void)
{
    /* ... */
}

// 处理SIGTERM信号的函数
/**
 * @brief 处理终止信号,记录日志并退出程序
 *
 * @param signo 信号编号
 */
// sigterm 函数用于处理 SIGTERM 信号
// 当程序接收到 SIGTERM 信号时,会调用此函数
// 参数 signo 是接收到的信号编号
void sigterm(int signo)
{
    // 使用 syslog 记录日志信息,表明收到了 SIGTERM 信号,并准备退出
    syslog(LOG_INFO, "got SIGTERM; exiting");

    // 调用 exit 函数退出程序,参数 0 表示正常退出
    exit(0);
}

// 处理SIGHUP信号的函数
/**
 * @brief 处理挂起信号,重新读取配置文件
 *
 * @param signo 信号编号
 */
// sighup函数用于处理SIGHUP信号
// 当接收到SIGHUP信号时,系统会调用此函数
// 函数内部会记录日志信息,并重新读取配置文件
void sighup(int signo)
{
    // 使用syslog记录信息级别为LOG_INFO的日志,内容为"Re-reading configuration file"
    syslog(LOG_INFO, "Re-reading configuration file");

    // 调用reread函数重新读取配置文件
    reread();
}

int main(int argc, char *argv[])
{
    char            *cmd;  // 声明一个字符指针,用于存储命令名称
    struct sigaction sa;   // 声明一个信号动作结构体,用于处理信号

    // 获取命令名称
    // 如果argv[0]中包含'/',则cmd指向最后一个'/'之后的字符串,否则直接使用argv[0]
    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;  // 如果找到了'/',则将cmd指针向后移动一位,跳过'/'

    // 成为守护进程
    // 调用daemonize函数,传入命令名称,使当前进程成为守护进程
    daemonize(cmd);

    // 确保只有一个守护进程实例在运行
    // 调用already_running函数检查是否已经有守护进程在运行
    if (already_running())
    {
        syslog(LOG_ERR, "daemon already running");  // 如果已经有守护进程在运行,则记录错误日志
        exit(1);                                    // 退出当前进程
    }

    // 设置信号处理
    // 为SIGTERM信号设置处理函数
    sa.sa_handler = sigterm;                // 设置SIGTERM信号的处理函数为sigterm
    sigemptyset(&sa.sa_mask);               // 清空信号掩码
    sigaddset(&sa.sa_mask, SIGHUP);         // 将SIGHUP信号添加到信号掩码中
    sa.sa_flags = 0;                        // 设置信号处理标志为默认值
    if (sigaction(SIGTERM, &sa, NULL) < 0)  // 注册SIGTERM信号的处理函数
    {
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));  // 如果注册失败,记录错误日志
        exit(1);                                                      // 退出程序
    }

    // 为SIGHUP信号设置处理函数
    sa.sa_handler = sighup;                // 设置SIGHUP信号的处理函数为sighup
    sigemptyset(&sa.sa_mask);              // 再次清空信号掩码
    sigaddset(&sa.sa_mask, SIGTERM);       // 将SIGTERM信号添加到信号掩码中
    sa.sa_flags = 0;                       // 保持信号处理标志为默认值
    if (sigaction(SIGHUP, &sa, NULL) < 0)  // 注册SIGHUP信号的处理函数
    {
        syslog(LOG_ERR, "can't catch SIGHUP: %s", strerror(errno));  // 如果注册失败,记录错误日志
        exit(1);                                                     // 退出程序
    }

    // 继续执行守护进程的其他部分
    /* ... */
    exit(0);
}
