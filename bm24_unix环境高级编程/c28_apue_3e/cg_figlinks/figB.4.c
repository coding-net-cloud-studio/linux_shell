/*
 * 错误处理例程,适用于可以作为守护进程运行的程序.
 */

#include "apue.h"
#include <errno.h>  /* errno 的定义 */
#include <stdarg.h> /* ISO C 可变参数 */
#include <syslog.h>

// 内部函数,用于实际的日志记录操作
static void log_doit(int, int, int, const char *, va_list ap);

/*
 * 调用者必须定义并设置此变量:如果为非零值表示交互式运行,为零表示守护进程模式
 */
extern int log_to_stderr;

/*
 * 初始化 syslog(),如果作为守护进程运行.
 * @param ident 系统日志的标识符
 * @param option syslog() 的选项
 * @param facility 系统日志的设施
 */
/**
 * @brief 打开系统日志功能
 *
 * 该函数用于打开系统日志功能,如果log_to_stderr为0,则调用openlog函数.
 *
 * @param ident 日志标识符,通常为程序名称
 * @param option 日志选项,例如LOG_PID等
 * @param facility 日志设施,例如LOG_USER等
 */
void log_open(const char *ident, int option, int facility)
{
    // 如果log_to_stderr为0,则打开系统日志
    if (log_to_stderr == 0)
        openlog(ident, option, facility);
}

/*
 * 与系统调用相关的非致命错误.
 * 打印带有系统 errno 值的消息并返回.
 * @param fmt 格式化字符串
 */

// log_ret函数用于记录错误日志,它接受一个格式化字符串和可变参数列表.
// 该函数首先初始化一个va_list类型的变量ap,用于访问可变参数列表.
// 然后调用log_doit函数,将错误级别设置为LOG_ERR,并传递errno值以便记录系统错误代码.
// 最后,清理资源,结束可变参数列表的处理.
void log_ret(const char *fmt, ...)
{
    va_list ap;  // 初始化可变参数列表

    va_start(ap, fmt);                     // 开始处理可变参数列表
    log_doit(1, errno, LOG_ERR, fmt, ap);  // 记录错误日志
    va_end(ap);                            // 结束处理可变参数列表
}

/*
 * 与系统调用相关的致命错误.
 * 打印消息并终止程序.
 * @param fmt 格式化字符串
 */

// log_sys函数用于记录系统错误日志,并在记录后退出程序.
// 参数fmt是日志消息的格式字符串,...表示可变参数列表.
void log_sys(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表的信息.

    va_start(ap, fmt);                     // 使用va_start宏初始化ap,第二个参数是可变参数前的最后一个固定参数.
    log_doit(1, errno, LOG_ERR, fmt, ap);  // 调用log_doit函数记录日志,参数包括日志级别,错误号,日志类型,格式字符串和可变参数列表.
    va_end(ap);                            // 使用va_end宏清理资源,结束对可变参数列表的处理.
    exit(2);                               // 记录日志后退出程序,返回状态码2.
}

/*
 * 与系统调用无关的非致命错误.
 * 打印消息并返回.
 * @param fmt 格式化字符串
 */

// log_msg函数用于记录日志消息.
// 它接受一个格式化字符串fmt和可变参数列表.
// 该函数通过调用log_doit来实际执行日志记录.
void log_msg(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表.

    va_start(ap, fmt);                 // 使用va_start宏初始化ap,使其指向第一个可变参数.
    log_doit(0, 0, LOG_ERR, fmt, ap);  // 调用log_doit函数记录日志,其中LOG_ERR是日志级别.
    va_end(ap);                        // 使用va_end宏清理资源,结束可变参数的处理.
}

/*
 * 与系统调用无关的致命错误.
 * 打印消息并终止程序.
 * @param fmt 格式化字符串
 */
// log_quit 函数用于记录错误信息并退出程序.
// 参数 fmt 是一个格式化字符串,后面的参数将按照 fmt 的格式进行格式化.
// 该函数使用可变参数列表来处理不确定数量的参数.
void log_quit(const char *fmt, ...)
{
    va_list ap;  // 定义一个 va_list 类型的变量 ap,用于存储可变参数列表

    va_start(ap, fmt);                 // 使用 va_start 宏初始化 ap,第二个参数是可变参数前的最后一个固定参数
    log_doit(0, 0, LOG_ERR, fmt, ap);  // 调用 log_doit 函数记录错误信息,其中 LOG_ERR 是错误级别
    va_end(ap);                        // 使用 va_end 宏清理 ap,结束可变参数列表的处理
    exit(2);                           // 退出程序,返回状态码 2 表示异常退出
}

/*
 * 与系统调用相关的致命错误.
 * 错误号作为显式参数传递.
 * 打印消息并终止程序.
 * @param error 错误号
 * @param fmt 格式化字符串
 */

/**
 * @brief 记录错误信息并退出程序
 *
 * @param error 错误代码
 * @param fmt 格式化字符串,用于描述错误信息
 * @param ... 可变参数列表,用于填充格式化字符串
 *
 * 该函数首先使用va_list和va_start宏来处理可变参数列表,
 * 然后调用log_doit函数记录错误信息,最后使用exit函数退出程序.
 */
void log_exit(int error, const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);                     // 使用va_start宏初始化ap,第二个参数是可变参数前的最后一个固定参数
    log_doit(1, error, LOG_ERR, fmt, ap);  // 调用log_doit函数记录错误信息
    va_end(ap);                            // 使用va_end宏清理资源
    exit(2);                               // 退出程序,返回状态码2表示异常退出
}

/*
 * 打印消息并返回给调用者.
 * 调用者指定 "errnoflag" 和 "priority".
 * @param errnoflag 是否包含错误号标志
 * @param error 错误号
 * @param priority 日志优先级
 * @param fmt 格式化字符串
 * @param ap 可变参数列表
 */
static void
log_doit(int errnoflag, int error, int priority, const char *fmt, va_list ap)
/*
 * log_doit - 记录日志信息
 * @errnoflag: 是否包含错误号标志
 * @error: 错误号
 * @priority: 日志优先级
 * @fmt: 格式化字符串
 * @ap: 可变参数列表
 *
 * 该函数用于记录日志信息,可以选择是否包含错误号,并根据优先级决定是输出到标准错误流还是系统日志.
 */
{
    char buf[MAXLINE];  // 定义缓冲区用于存储格式化后的日志信息

    vsnprintf(buf, MAXLINE - 1, fmt, ap);                                                 // 将格式化字符串写入缓冲区
    if (errnoflag)                                                                        // 如果需要包含错误号
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s", strerror(error));  // 将错误信息追加到缓冲区
    strcat(buf, "\n");                                                                    // 在缓冲区末尾添加换行符

    if (log_to_stderr)  // 如果配置为输出到标准错误流
    {
        fflush(stdout);      // 刷新标准输出流
        fputs(buf, stderr);  // 将日志信息写入标准错误流
        fflush(stderr);      // 刷新标准错误流
    }
    else  // 否则输出到系统日志
    {
        syslog(priority, "%s", buf);  // 根据优先级记录日志
    }
}
