#include "open.h"   // 包含自定义的头文件open.h,可能包含一些自定义的函数声明或宏定义
#include <fcntl.h>  // 包含标准库中的fcntl.h头文件,提供了文件控制操作的功能

#define BUFFSIZE 8192  // 定义了一个常量BUFFSIZE,值为8192,通常用于定义缓冲区大小

int main(int argc, char *argv[])
{
    int  n, fd;                         // n: 读取的字节数, fd: 文件描述符
    char buf[BUFFSIZE], line[MAXLINE];  // buf: 缓冲区用于存储读取的数据, line: 存储从stdin读取的文件名

    /* 从stdin读取要cat的文件名 */
    // 从标准输入读取一行文本,直到遇到文件结束符或发生错误
    // fgets函数尝试从指定的流中读取一行,并存储到line指向的数组中
    // MAXLINE定义了可以读取的最大字符数,包括终止的空字符
    // 如果成功读取一行,fgets返回指向line的指针;如果遇到文件结束或读取错误,则返回NULL
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        // 循环体,处理每一行读取的内容

        // 检查字符串末尾是否有换行符,如果有则将其移除
        // 这样做可以确保字符串在处理时不会因为末尾的换行符而出错
        if (line[strlen(line) - 1] == '\n')  // 检查并移除行尾的换行符
            line[strlen(line) - 1] = 0;

        /* 打开文件 */
        if ((fd = csopen(line, O_RDONLY)) < 0)  // 使用csopen()尝试以只读模式打开文件
            continue;                           // 如果打开失败,打印错误信息并继续下一次循环

        /* 并将内容cat到stdout */
        while ((n = read(fd, buf, BUFFSIZE)) > 0)   // 从文件描述符fd读取数据到buf
            if (write(STDOUT_FILENO, buf, n) != n)  // 将buf中的数据写入stdout
                err_sys("write error");             // 如果写入失败,打印错误信息
        if (n < 0)                                  // 如果读取过程中发生错误
            err_sys("read error");                  // 打印错误信息
        close(fd);                                  // 关闭文件描述符
    }

    exit(0);  // 程序正常退出
}
