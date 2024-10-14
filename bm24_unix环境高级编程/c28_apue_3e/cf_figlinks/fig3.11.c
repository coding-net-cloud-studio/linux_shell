#include "apue.h"
#include <fcntl.h>

// 主函数,用于获取文件描述符的访问模式和标志
int main(int argc, char *argv[])
{
    int val;  // 存储fcntl函数返回的值

    // 检查命令行参数的数量是否正确
    if (argc != 2)
        err_quit("usage: a.out <descriptor#>");  // 如果不正确,输出使用方法并退出

    // 使用fcntl函数获取文件描述符的状态标志
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        err_sys("fcntl error for fd %d", atoi(argv[1]));  // 如果出错,输出错误信息并退出

    // 根据返回的状态标志中的访问模式进行分类
    switch (val & O_ACCMODE)
    {
    case O_RDONLY:
        printf("read only");  // 只读模式
        break;
    case O_WRONLY:
        printf("write only");  // 只写模式
        break;
    case O_RDWR:
        printf("read write");  // 读写模式
        break;
    default:
        err_dump("unknown access mode");  // 未知访问模式,输出错误信息并退出
    }

    // 检查并输出其他标志
    if (val & O_APPEND)
        printf(", append");  // 追加模式
    if (val & O_NONBLOCK)
        printf(", nonblocking");  // 非阻塞模式
    if (val & O_SYNC)
        printf(", synchronous writes");  // 同步写入

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    // 如果定义了O_FSYNC且不等于O_SYNC,则检查并输出同步写入标志
    if (val & O_FSYNC)
        printf(", synchronous writes");
#endif

    putchar('\n');  // 输出换行符
    exit(0);        // 正常退出程序
}
