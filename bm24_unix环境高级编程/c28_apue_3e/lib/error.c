#include "apue.h"                                  // 引入APUE库头文件,提供系统编程所需的接口
#include <errno.h> /* for definition of errno */   // 引入errno头文件,定义了全局变量errno,用于报告错误
#include <stdarg.h> /* ISO C variable aruments */  // 引入stdarg.h头文件,支持可变参数列表

// 内部函数,用于打印错误信息
static void err_doit(int, int, const char *, va_list);

/*
 * 非致命的系统调用错误.
 * 打印消息并返回.
 */
// error.c

/**
 * @brief 输出错误信息,但不终止程序执行.
 *
 * 该函数用于格式化并输出错误信息,使用标准的错误处理机制.
 * 它不会导致程序退出,而是将错误信息传递给err_doit函数处理.
 *
 * @param fmt 格式化字符串,类似于printf函数的格式化参数.
 */
void err_ret(const char *fmt, ...)
{
    va_list ap;  // 变长参数列表

    va_start(ap, fmt);            // 初始化变长参数列表
    err_doit(1, errno, fmt, ap);  // 调用err_doit函数处理错误
    va_end(ap);                   // 清理变长参数列表
}

/*
 * 致命的系统调用错误.
 * 打印消息并终止程序.
 */
// error.c

/**
 * @brief 处理系统错误的函数,将错误信息打印到标准错误输出,并退出程序.
 *
 * @param fmt 格式化字符串,用于构造错误消息.
 * @param ... 可变参数列表,用于格式化字符串.
 */
void err_sys(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);            // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(1, errno, fmt, ap);  // 调用err_doit函数,处理错误信息
    va_end(ap);                   // 清理资源,结束可变参数的处理
    exit(1);                      // 退出程序,1表示异常退出
}

/*
 * 非致命的与系统调用无关的错误.
 * 错误代码作为显式参数传递.
 * 打印消息并返回.
 */
// error.c

// err_cont函数用于处理错误信息,它接受一个错误码,一个格式化字符串以及可变参数列表.
// 该函数首先初始化一个va_list类型的变量ap来存储可变参数列表.
// 然后调用err_doit函数来处理错误,最后清理va_list变量.
void err_cont(int error, const char *fmt, ...)
{
    va_list ap;  // 初始化一个va_list类型的变量ap

    va_start(ap, fmt);            // 使用fmt初始化ap,准备访问可变参数列表
    err_doit(1, error, fmt, ap);  // 调用err_doit函数处理错误,传入错误码,格式化字符串和参数列表
    va_end(ap);                   // 清理va_list变量,释放相关资源
}

/*
 * 致命的与系统调用无关的错误.
 * 错误代码作为显式参数传递.
 * 打印消息并终止程序.
 */
// error.c

// err_exit函数用于在发生错误时打印错误信息并退出程序.
// 参数error是错误代码,fmt是格式化字符串,...是与fmt对应的可变参数列表.
void err_exit(int error, const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表的信息

    va_start(ap, fmt);            // 使用va_start宏初始化ap,第二个参数是可变参数列表前的最后一个固定参数
    err_doit(1, error, fmt, ap);  // 调用err_doit函数,传入错误输出的文件描述符(1表示标准错误输出),错误代码,格式化字符串和可变参数列表
    va_end(ap);                   // 使用va_end宏清理资源,结束对可变参数列表的处理
    exit(1);                      // 调用exit函数退出程序,参数1表示程序异常退出
}

/*
 * 致命的系统调用错误.
 * 打印消息,转储核心,并终止程序.
 */
// error.c

/**
 * @brief 输出错误信息并终止程序
 *
 * 该函数用于在发生严重错误时输出错误信息,并终止程序.
 * 它首先使用可变参数列表来处理错误信息的格式化输出,
 * 然后调用err_doit函数进行实际的错误输出操作.
 * 最后,它会调用abort函数来转储核心并终止程序.
 * 注意:exit(1)语句永远不会被执行,因为在abort之后程序会立即终止.
 *
 * @param fmt 格式化字符串,用于定义错误信息的输出格式
 * @param ... 可变参数列表,与fmt中的格式化占位符对应
 */
void err_dump(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);             // 初始化ap,使其指向第一个可变参数
    err_doit(1, errno, fmt, ap);   // 调用err_doit函数,输出错误信息
    va_end(ap);                    // 清理资源,结束可变参数列表的处理
    abort(); /* 转储核心并终止 */  // 终止程序,不返回
    exit(1); /* 不应该到达这里 */  // 该语句永远不会被执行
}

/*
 * 非致命的与系统调用无关的错误.
 * 打印消息并返回.
 */
// error.c

// err_msg函数用于输出错误信息
// 参数fmt是格式化字符串,用于定义输出信息的格式
// 其他参数通过...传递,数量和类型由fmt指定
void err_msg(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量ap,用于存储可变参数列表

    va_start(ap, fmt);        // 使用va_start宏初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(0, 0, fmt, ap);  // 调用err_doit函数,传递错误信息的格式化字符串和参数列表
    va_end(ap);               // 使用va_end宏清理资源,结束可变参数的处理
}

/*
 * 致命的与系统调用无关的错误.
 * 打印消息并终止程序.
 */
// error.c

// @brief 输出错误信息并退出程序
//
// 该函数用于在发生严重错误时输出格式化的错误信息,并终止程序执行.
// 它使用可变参数列表来处理不同数量的参数.
//
// @param fmt 格式化字符串,用于定义输出的错误信息格式
// @param ... 可变参数列表,根据fmt中的格式化指令提供相应的参数
void err_quit(const char *fmt, ...)
{
    va_list ap;  // 定义一个va_list类型的变量,用于存储可变参数列表

    va_start(ap, fmt);        // 初始化ap,第二个参数是可变参数前的最后一个固定参数
    err_doit(0, 0, fmt, ap);  // 调用err_doit函数,处理错误信息的输出
    va_end(ap);               // 清理资源,结束可变参数列表的处理
    exit(1);                  // 退出程序,返回状态码1表示异常退出
}

/*
 * 打印消息并返回给调用者.
 * 调用者指定 "errnoflag".
 */
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char buf[MAXLINE];  // 定义一个字符数组用于存储错误消息

    vsnprintf(buf, MAXLINE - 1, fmt, ap);  // 使用vsnprintf函数格式化错误消息到buf中,防止缓冲区溢出
    // vsnprintf函数说明: 将格式化的字符串写入buf,最多写入MAXLINE-1个字符,以防止溢出,ap是可变参数列表

    if (errnoflag)                                                                        // 如果errnoflag为真,表示需要添加错误代码描述
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s", strerror(error));  // 使用snprintf函数将错误代码描述追加到buf中
    // snprintf函数说明: 将格式化的错误代码描述追加到buf的末尾,最多写入MAXLINE-strlen(buf)-1个字符,以防止溢出

    strcat(buf, "\n");  // 在错误消息末尾添加换行符
    // strcat函数说明: 将换行符'\n'追加到buf的末尾

    fflush(stdout); /* in case stdout and stderr are the same */  // 刷新标准输出流,以防stdout和stderr指向同一设备
    // fflush函数说明: 刷新流,确保所有待输出的数据都被写入到对应的设备

    fputs(buf, stderr);  // 将错误消息写入标准错误流
    // fputs函数说明: 将buf中的字符串写入到stderr流中

    fflush(NULL); /* flushes all stdio output streams */  // 刷新所有标准IO流,确保错误消息被立即输出
    // fflush函数说明: 刷新所有标准IO流,确保所有待输出的数据都被立即写入到对应的设备
}
