#include "apue.h"
#include <fcntl.h>

int main(void)
{
    int i, n;   // i用于存储write函数的返回值,n用于记录写入的字节数
    int fd[2];  // fd数组用于存储pipe函数创建的管道文件描述符

    if (pipe(fd) < 0)  // 创建管道,如果失败则调用err_sys函数报告错误
        err_sys("pipe error");
    set_fl(fd[1], O_NONBLOCK);  // 设置管道的写端为非阻塞模式

    /* write 1 byte at a time until pipe is full */  // 循环写入数据直到管道满
    for (n = 0;; n++)
    {
        if ((i = write(fd[1], "a", 1)) != 1)  // 尝试写入一个字节的数据
        {
            printf("write ret %d, ", i);  // 如果write返回值不为1,打印返回值并跳出循环
            break;
        }
    }
    printf("pipe capacity = %d\n", n);  // 打印管道的容量
    exit(0);                            // 正常退出程序
}
