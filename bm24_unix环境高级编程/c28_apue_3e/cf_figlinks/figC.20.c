#include "apue.h"   // 引入apue库的头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

#define FIFO "temp.fifo"  // 定义一个宏FIFO,表示将要创建或操作的FIFO文件的名称

int main(void)
{
    // 定义文件描述符fdread和fdwrite
    // 声明两个整数变量,分别用于存储读和写FIFO文件的文件描述符
    int fdread, fdwrite;

    // 删除已存在的FIFO文件
    unlink(FIFO);
    // 创建一个新的FIFO文件,如果失败则调用err_sys函数输出错误信息并终止程序
    if (mkfifo(FIFO, FILE_MODE) < 0)
        err_sys("mkfifo error");
    // 以只读和非阻塞的方式打开FIFO文件,如果失败则调用err_sys函数输出错误信息并终止程序
    if ((fdread = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0)
        err_sys("open error for reading");
    // 以只写的方式打开FIFO文件,如果失败则调用err_sys函数输出错误信息并终止程序
    if ((fdwrite = open(FIFO, O_WRONLY)) < 0)
        err_sys("open error for writing");
    // 清除fdread文件描述符的非阻塞标志
    clr_fl(fdread, O_NONBLOCK);
    // 程序正常结束,返回0
    exit(0);
}
