#include "opend.h"  // 引入自定义的头文件opend.h,可能包含了程序所需的函数声明和定义.

// errmsg 用于存储错误信息的字符数组,最大长度为 MAXLINE
char errmsg[MAXLINE];
// oflag 用于存储文件打开标志的整型变量
int oflag;
// pathname 指向存储文件路径的字符串指针
char *pathname;

/**
 * 主函数,用于从标准输入读取数据,处理请求,并将结果输出到标准输出.
 */
int main(void)
{
    int  nread;         // 读取的字节数
    char buf[MAXLINE];  // 用于存储从客户端读取的数据的缓冲区

    // 无限循环,直到客户端关闭流管道
    for (;;)
    {
        // 从标准输入读取数据到缓冲区
        if ((nread = read(STDIN_FILENO, buf, MAXLINE)) < 0)
            err_sys("read error on stream pipe");  // 如果读取失败,输出错误信息并退出
        else if (nread == 0)
            break;                                  // 如果读取的字节数为0,表示客户端已关闭流管道,退出循环
        handle_request(buf, nread, STDOUT_FILENO);  // 处理请求并将结果输出到标准输出
    }
    exit(0);  // 程序正常退出
}
