/*
 * Error routines for programs that can run as a daemon.
 */

// 包含APUE库的头文件,该库提供了一套用于编写Unix/Linux程序的API
#include "apue.h"

// 包含errno的定义,errno是一个全局变量,用于表示最近一次系统调用的错误代码
#include <errno.h>

// 包含ISO C可变参数列表的定义,用于处理不确定数量的参数
#include <stdarg.h>

// 包含syslog函数的定义,syslog用于记录系统消息
#include <syslog.h>

// 定义一个静态函数log_doit,用于记录日志的具体操作
// 参数包括:设施等级(facility),错误级别(severity),系统错误号(errno),格式化字符串(format),可变参数列表(ap)
static void log_doit(int facility, int severity, int err, const char *format, va_list ap);

/*
 * Caller must define and set this: nonzero if
 * interactive, zero if daemon
 */

/*
 * 调用者必须定义并设置此变量:如果为交互式则为非零值,如果为守护进程则为零
 */

extern int log_to_stderr;  // 声明一个外部变量log_to_stderr,用于标识程序是否运行在交互模式下(非零值)或守护进程模式(零值)

/*
 * Initialize syslog(), if running as daemon.
 */

/*
 * 初始化syslog(),如果作为守护进程运行.
 *
 * 如果程序作为守护进程运行,需要初始化syslog以便记录系统消息.
 * syslog()函数用于将消息发送到系统日志.
 * 这个函数应该在程序的早期调用,以确保所有的日志消息都能被捕获.
 */

void log_open(const char *ident, int option, int facility)  // 定义一个函数log_open,用于初始化syslog服务
{
    if (log_to_stderr == 0)                // 如果程序运行在守护进程模式下
        openlog(ident, option, facility);  // 调用openlog函数初始化syslog
}

/*
 * 非致命的系统调用错误.
 * 打印带有系统errno值的消息并返回.
 */
void log_ret(const char *fmt, ...)
{
    // va_list 是一个字符指针类型,用于指向可变参数列表
    va_list ap;

    // 初始化可变参数列表
    va_start(ap, fmt);
    // 调用log_doit函数记录错误信息
    log_doit(1, errno, LOG_ERR, fmt, ap);
    // 清理可变参数列表
    va_end(ap);
}

/*
 * 致命的系统调用错误.
 * 打印消息并终止程序.
 */
void log_sys(const char *fmt, ...)
{
    // va_list 是一个字符指针类型,用于指向可变参数列表
    va_list ap;

    // 初始化可变参数列表
    va_start(ap, fmt);
    // 调用log_doit函数记录错误信息
    log_doit(1, errno, LOG_ERR, fmt, ap);
    // 清理可变参数列表
    va_end(ap);
    // 终止程序,返回状态码2
    exit(2);
}

/*
 * 非致命错误,与系统调用无关.
 * 打印消息并返回.
 */
void log_msg(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表

    va_start(ap, fmt);                 // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    log_doit(0, 0, LOG_ERR, fmt, ap);  // 调用log_doit函数,处理日志消息
    va_end(ap);                        // 清理ap,结束可变参数的处理
}

/*
 * 致命错误,与系统调用无关.
 * 打印消息并终止程序.
 */
void log_quit(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表

    va_start(ap, fmt);                 // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    log_doit(0, 0, LOG_ERR, fmt, ap);  // 调用log_doit函数,处理日志消息
    va_end(ap);                        // 清理ap,结束可变参数的处理
    exit(2);                           // 终止程序,返回状态码2表示异常退出
}

/*
 * 与系统调用相关的致命错误.
 * 错误号作为显式参数传递.
 * 打印消息并终止程序.
 */
void log_exit(int error, const char *fmt, ...)
{
    va_list ap;

    // 初始化可变参数列表
    va_start(ap, fmt);
    // 调用log_doit函数记录错误信息
    log_doit(1, error, LOG_ERR, fmt, ap);
    // 清理可变参数列表
    va_end(ap);
    // 终止程序,返回状态码2
    exit(2);
}

/*
 * 打印消息并返回给调用者.
 * 调用者指定"errnoflag"和"priority".
 *
 * @param errnoflag 是否包含错误号标志
 * @param error 错误号
 * @param priority 日志优先级
 * @param fmt 格式化字符串
 * @param ap 可变参数列表
 */
static void
log_doit(int errnoflag, int error, int priority, const char *fmt, va_list ap)
{
    // buf 是一个字符数组,用于存储从输入读取的数据或要输出的数据.
    // MAXLINE 是一个预定义的常量,表示 buf 的最大长度.
    // 这个数组通常用于处理行缓冲输入输出.
    char buf[MAXLINE];

    // 将格式化的消息写入缓冲区
    vsnprintf(buf, MAXLINE - 1, fmt, ap);
    // 如果errnoflag为真,则附加错误描述
    if (errnoflag)
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s", strerror(error));
    // 在消息后附加换行符
    strcat(buf, "\n");
    // 如果log_to_stderr为真,则将错误消息输出到stderr
    // 如果log_to_stderr为真,则将错误消息输出到标准错误流
    if (log_to_stderr)
    {
        fflush(stdout);      // 刷新标准输出流,确保之前的输出已经写入
        fputs(buf, stderr);  // 将错误消息buf写入标准错误流
        fflush(stderr);      // 刷新标准错误流,确保错误消息已经写入
    }
    // 否则,使用syslog记录错误消息
    else
    {
        syslog(priority, "%s", buf);  // 使用syslog函数记录错误消息,priority指定日志的优先级
    }
}
