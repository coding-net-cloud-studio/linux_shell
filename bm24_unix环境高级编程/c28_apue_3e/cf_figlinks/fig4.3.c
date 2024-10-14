#include "apue.h"

// 主函数,用于判断命令行参数指定的文件类型
int main(int argc, char *argv[])
{
    int         i;    // 循环变量
    struct stat buf;  // 用于存储文件状态信息的结构体
    char       *ptr;  // 指向文件类型字符串的指针

    // 遍历命令行参数(跳过程序名本身)
    for (i = 1; i < argc; i++)
    {
        printf("%s: ", argv[i]);  // 打印文件名

        // 获取文件状态信息
        if (lstat(argv[i], &buf) < 0)
        {
            err_ret("lstat error");  // 如果出错,打印错误信息并继续下一个文件
            continue;
        }

        // 根据文件类型设置相应的字符串指针
        if (S_ISREG(buf.st_mode))
            ptr = "regular";  // 普通文件
        else if (S_ISDIR(buf.st_mode))
            ptr = "directory";  // 目录
        else if (S_ISCHR(buf.st_mode))
            ptr = "character special";  // 字符设备文件
        else if (S_ISBLK(buf.st_mode))
            ptr = "block special";  // 块设备文件
        else if (S_ISFIFO(buf.st_mode))
            ptr = "fifo";  // 管道
        else if (S_ISLNK(buf.st_mode))
            ptr = "symbolic link";  // 符号链接
        else if (S_ISSOCK(buf.st_mode))
            ptr = "socket";  // 套接字
        else
            ptr = "** unknown mode **";  // 未知类型

        printf("%s\n", ptr);  // 打印文件类型
    }
    exit(0);  // 程序正常退出
}
