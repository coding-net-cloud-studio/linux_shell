#include "apue.h"   // 引入apue库,提供系统编程接口
#include <errno.h>  // 引入errno头文件,定义了错误码
#include <fcntl.h>  // 引入fcntl头文件,提供了文件控制操作

char buf[500000];  // 定义一个足够大的缓冲区来存储从标准输入读取的数据

int main(void)
{
    int   ntowrite, nwrite;  // ntowrite是要写入的字节数,nwrite是实际写入的字节数
    char *ptr;               // ptr是指向buf中当前要写入部分的指针

    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));  // 从标准输入读取数据到buf中
    fprintf(stderr, "read %d bytes\n", ntowrite);     // 输出读取的字节数

    set_fl(STDOUT_FILENO, O_NONBLOCK);  // 设置标准输出为非阻塞模式

    ptr = buf;  // 初始化ptr指向buf的开始位置
    while (ntowrite > 0)
    {                                                                 // 当还有数据要写入时循环
        errno  = 0;                                                   // 清除errno,以便检查write函数的错误
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);                 // 尝试写入数据
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);  // 输出实际写入的字节数和errno

        if (nwrite > 0)
        {                        // 如果有数据成功写入
            ptr += nwrite;       // 移动ptr到下一个要写入的位置
            ntowrite -= nwrite;  // 减少剩余要写入的字节数
        }
    }

    clr_fl(STDOUT_FILENO, O_NONBLOCK);  // 清除标准输出的非阻塞模式

    exit(0);  // 正常退出程序
}
