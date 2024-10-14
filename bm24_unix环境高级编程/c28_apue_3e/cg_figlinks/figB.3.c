#include "apue.h"                                  // 包含APUE库的头文件
#include <errno.h> /* for definition of errno */   // 包含errno定义的头文件,用于错误处理
#include <stdarg.h> /* ISO C variable aruments */  // 包含可变参数列表定义的头文件

// 静态函数err_doit,用于处理错误信息的输出
// int errnoflag: 如果非零,则在消息前输出errno的值
// int level: 错误级别
// const char *fmt: 格式化字符串
// va_list ap: 可变参数列表
static void err_doit(int errnoflag, int level, const char *fmt, va_list ap);

/*
 * 非致命性系统调用错误.
 * 打印消息并返回.
 *
 * @param fmt 格式化字符串
 * @param ... 可变参数列表
 */
void err_ret(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(1, errno, fmt, ap);  // 调用err_doit函数,处理错误信息
    va_end(ap);                   // 清理资源,结束可变参数的处理
}

/*
 * 当系统调用发生致命错误时调用.
 * 打印错误信息并终止程序.
 *
 * @param fmt 格式化字符串,用于输出错误信息
 * @param ... 可变参数列表,与fmt中的格式化占位符对应
 */
void err_sys(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(1, errno, fmt, ap);  // 调用err_doit函数,处理错误信息
    va_end(ap);                   // 清理资源,结束可变参数的处理
    exit(1);                      // 终止程序,退出码为1表示异常退出
}

/*
 * 非致命错误,与系统调用无关.
 * 错误代码作为显式参数传递.
 * 打印消息并返回.
 */
void err_cont(int error, const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,第二个参数fmt是可变参数前的最后一个固定参数
    err_doit(1, error, fmt, ap);  // 调用err_doit函数,处理错误信息
    va_end(ap);                   // 清理ap,释放相关资源
}

/*
 * 当发生与系统调用无关的致命错误时调用此函数.
 * 错误代码作为显式参数传递.
 * 打印消息并终止程序.
 *
 * @param error 错误代码
 * @param fmt 格式化字符串,用于打印错误信息
 * @param ... 可变参数列表,与fmt中的格式说明符对应
 */
void err_exit(int error, const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,使其指向第一个可变参数
    err_doit(1, error, fmt, ap);  // 调用err_doit函数处理错误
    va_end(ap);                   // 清理资源,结束可变参数的处理
    exit(1);                      // 终止程序
}

/*
 * 当系统调用发生致命错误时.
 * 打印消息,转储核心,并终止程序.
 */
void err_dump(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(1, errno, fmt, ap);  // 调用err_doit函数,传入错误级别,错误编号,格式化字符串和参数列表
    va_end(ap);                   // 清理ap,释放资源
    abort();                      // 转储核心并终止程序
    exit(1);                      // 理论上不会执行到这里,因为abort已经终止了程序
}

/*
 * 非致命错误,与系统调用无关.
 * 打印一条消息并返回.
 *
 * @param fmt 格式化字符串
 * @param ... 可变参数列表
 */
void err_msg(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);        // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(0, 0, fmt, ap);  // 调用err_doit函数处理错误消息
    va_end(ap);               // 清理资源,结束可变参数的处理
}

/*
 * 当发生与系统调用无关的致命错误时,打印消息并终止程序.
 *
 * @param fmt 格式化字符串,用于输出错误信息
 * @param ... 可变参数列表,用于格式化字符串中的参数
 */
void err_quit(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);        // 初始化ap,使其指向第一个可变参数
    err_doit(0, 0, fmt, ap);  // 调用err_doit函数,处理错误信息的输出
    va_end(ap);               // 清理资源,结束可变参数的处理
    exit(1);                  // 终止程序,返回非零状态码表示异常退出
}

/*
 * 打印消息并返回给调用者.
 * 调用者指定 "errnoflag".
 *
 * @param errnoflag 如果非零,则在消息后附加错误描述
 * @param error 错误代码
 * @param fmt 格式化字符串
 * @param ap 可变参数列表
 */
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char buf[MAXLINE];  // 定义缓冲区用于存储格式化后的消息

    vsnprintf(buf, MAXLINE - 1, fmt, ap);                                                 // 将格式化的消息写入缓冲区
    if (errnoflag)                                                                        // 如果errnoflag非零
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s", strerror(error));  // 在消息后附加错误描述
    strcat(buf, "\n");                                                                    // 在消息末尾添加换行符
    fflush(stdout);                                                                       // 刷新标准输出流,以防它与标准错误流相同
    fputs(buf, stderr);                                                                   // 将消息写入标准错误流
    fflush(NULL);                                                                         // 刷新所有标准IO输出流
}
