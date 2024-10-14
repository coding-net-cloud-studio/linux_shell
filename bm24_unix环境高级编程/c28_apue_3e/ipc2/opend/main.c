#include "opend.h"   // 包含自定义头文件opend.h
#include <syslog.h>  // 包含系统日志库头文件

// 定义全局变量
int     debug;            // 调试标志
int     oflag;            // 打开文件标志
int     client_size;      // 客户端大小
int     log_to_stderr;    // 是否将日志输出到标准错误
char    errmsg[MAXLINE];  // 错误消息缓冲区
char   *pathname;         // 路径名指针
Client *client = NULL;    // 客户端结构体指针

int main(int argc, char *argv[])
{
    int c;

    // 打开日志文件,记录进程ID和用户信息
    log_open("open.serv", LOG_PID, LOG_USER);

    // 禁止getopt()函数将错误信息输出到标准错误流
    opterr = 0;
    // 解析命令行参数
    // getopt 函数用于解析命令行参数.
    // argc: 命令行参数的个数,包括程序名本身.
    // argv: 指向命令行参数字符串数组的指针.
    // "d": 表示我们期望一个选项字符 'd'.
    while ((c = getopt(argc, argv, "d")) != EOF)
    {
        // 循环直到没有更多的选项参数.
        switch (c)
        {
        case 'd':                       // 调试模式
            debug = log_to_stderr = 1;  // 设置调试标志,并将日志输出到标准错误流
            break;

        case '?':                                          // 未识别的选项
            err_quit("unrecognized option: -%c", optopt);  // 输出错误信息并退出程序
        }
    }

    // 如果不是调试模式,则以守护进程方式运行程序
    if (debug == 0)
        daemonize("opend");

    // 进入主循环,程序将在此处持续运行,不会返回
    loop();
}
