#include "apue.h"
#include <fcntl.h>

// 主函数,用于修改文件的时间戳
int main(int argc, char *argv[])
{
    int             i, fd;     // i为循环变量,fd为文件描述符
    struct stat     statbuf;   // 用于存储文件状态信息
    struct timespec times[2];  // 存储访问时间和修改时间

    // 遍历命令行参数中的文件名
    for (i = 1; i < argc; i++)
    {
        // 获取文件状态信息
        if (stat(argv[i], &statbuf) < 0)
        {
            /* fetch current times */
            err_ret("%s: stat error", argv[i]);  // 如果出错,打印错误信息并继续下一个文件
            continue;
        }
        // 打开文件,准备修改时间戳
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0)
        {
            /* truncate */
            err_ret("%s: open error", argv[i]);  // 如果出错,打印错误信息并继续下一个文件
            continue;
        }
        // 将文件的访问时间和修改时间复制到times数组
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        // 修改文件的时间戳
        if (futimens(fd, times) < 0)
            err_ret("%s: futimens error", argv[i]);  // 如果出错,打印错误信息
        close(fd);                                   // 关闭文件描述符
    }
    exit(0);  // 程序正常退出
}
