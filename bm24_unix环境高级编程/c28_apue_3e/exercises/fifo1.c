#include "apue.h"   // 引入APUE库,提供系统编程接口
#include <fcntl.h>  // 引入文件控制相关的头文件,用于文件操作

// 定义FIFO文件的名称
#define FIFO "temp.fifo"

int main(void)
{
    int fdread, fdwrite;

    // 如果FIFO文件已存在,则删除它
    unlink(FIFO);
    // 创建一个名为FIFO的FIFO文件,权限设置为FILE_MODE
    if (mkfifo(FIFO, FILE_MODE) < 0)
        // 如果创建失败,打印错误信息并退出程序
        err_sys("mkfifo error");
    // 以只读非阻塞模式打开FIFO文件
    if ((fdread = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0)
        // 如果打开失败,打印错误信息并退出程序
        err_sys("open error for reading");
    // 以只写模式打开FIFO文件
    if ((fdwrite = open(FIFO, O_WRONLY)) < 0)
        // 如果打开失败,打印错误信息并退出程序
        err_sys("open error for writing");
    // 清除fdread文件描述符的非阻塞标志
    clr_fl(fdread, O_NONBLOCK);
    // 程序正常退出
    exit(0);
}
