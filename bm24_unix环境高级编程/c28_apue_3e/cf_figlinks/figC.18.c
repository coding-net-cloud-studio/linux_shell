#include "apue.h"   // 引入APUE库的头文件,该库提供了一套用于UNIX和类UNIX系统的应用程序编程接口
#include <fcntl.h>  // 引入文件控制操作的头文件,包括文件打开,关闭,锁定等操作的定义

/**
 * @brief 主函数,程序的入口点
 *
 * 这个程序会创建一个管道,并尝试向管道中写入数据,直到管道满为止.然后,它会打印出管道的容量.
 *
 * @return int 程序的退出状态码
 */
int main(void)
{
    // 声明变量
    int i, n;   // i 和 n 用于循环和存储数据
    int fd[2];  // fd 是一个包含两个整数的数组,通常用于文件描述符

    // 创建一个管道,如果失败则调用 err_sys 函数输出错误信息并终止程序
    if (pipe(fd) < 0)
        err_sys("pipe error");

    // 设置管道写端为非阻塞模式
    set_fl(fd[1], O_NONBLOCK);

    // 尝试向管道中写入数据,直到管道满为止
    // 初始化计数器n为0
    for (n = 0;; n++)
    {
        // 向管道中写入一个字节的数据
        if ((i = write(fd[1], "a", 1)) != 1)
        {
            // 打印写入的字节数和返回值
            printf("write ret %d, ", i);
            // 跳出循环
            break;
        }
    }

    // 打印管道的容量
    printf("pipe capacity = %d\n", n);
    // 程序正常结束,返回 0
    exit(0);
}
