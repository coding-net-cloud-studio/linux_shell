#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口.
#include <fcntl.h>  // 引入文件控制操作的头文件,包含了文件打开,关闭,锁定等操作的函数原型.

// 主函数,用于获取文件描述符的状态标志并打印
int main(int argc, char *argv[])
{
    int val;  // 用于存储fcntl函数返回的标志值

    // 检查命令行参数的数量,如果不等于2,则打印错误信息并退出
    if (argc != 2)
        err_quit("usage: a.out <descriptor#>");

    // 调用fcntl函数获取文件描述符的状态标志
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        err_sys("fcntl error for fd %d", atoi(argv[1]));

    // 根据标志值判断文件访问模式并打印
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
        err_dump("unknown access mode");  // 未知访问模式
    }

    // 根据标志值判断是否有追加,非阻塞,同步写入等属性并打印
    // 检查文件标志位并打印相应的描述
    if (val & O_APPEND)
    {
        // 如果设置了O_APPEND标志,表示文件以追加模式打开
        printf(", append");
    }
    if (val & O_NONBLOCK)
    {
        // 如果设置了O_NONBLOCK标志,表示文件以非阻塞模式打开
        printf(", nonblocking");
    }
    if (val & O_SYNC)
    {
        // 如果设置了O_SYNC标志,表示写操作是同步的
        printf(", synchronous writes");
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    // 如果定义了O_FSYNC且不等于O_SYNC,则检查是否有同步写入属性并打印
    if (val & O_FSYNC)
        printf(", synchronous writes");
#endif

    putchar('\n');  // 打印换行符
    exit(0);        // 退出程序
}
