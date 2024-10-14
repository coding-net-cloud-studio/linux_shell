#include "apue.h"   // 引入APUE库,提供系统调用和实用函数的封装
#include <errno.h>  // 引入错误处理头文件,定义了errno变量和各种错误代码
#include <fcntl.h>  // 引入文件控制操作头文件,如open, fcntl等函数

char buf[500000];  // 定义一个足够大的缓冲区用于存储从标准输入读取的数据

int main(void)
{
    int   ntowrite, nwrite;  // ntowrite: 待写入的字节数, nwrite: 实际写入的字节数
    char *ptr;               // 指向当前待写入数据的首地址

    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));  // 从标准输入读取数据到缓冲区
    fprintf(stderr, "read %d bytes\n", ntowrite);     // 输出读取到的字节数

    set_fl(STDOUT_FILENO, O_NONBLOCK);  // 设置标准输出为非阻塞模式

    ptr = buf;            // 初始化指针指向缓冲区的开始位置
    while (ntowrite > 0)  // 当还有数据待写入时循环
    {
        errno  = 0;                                                   // 清空errno,用于检查write函数的错误
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);                 // 尝试写入数据到标准输出
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);  // 输出实际写入的字节数和错误码

        if (nwrite > 0)  // 如果有数据成功写入
        {
            ptr += nwrite;       // 移动指针到下一个待写入的位置
            ntowrite -= nwrite;  // 更新剩余待写入的字节数
        }
    }

    clr_fl(STDOUT_FILENO, O_NONBLOCK);  // 清除标准输出的非阻塞模式

    exit(0);  // 程序正常退出
}
