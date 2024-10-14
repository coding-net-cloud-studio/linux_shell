#include "open.h"   // 引入自定义的头文件open.h,可能包含一些自定义函数或宏定义
#include <fcntl.h>  // 引入标准库中的fcntl.h,提供文件控制功能

#define BUFFSIZE 8192  // 定义常量BUFFSIZE,表示缓冲区大小为8192字节

int main(int argc, char *argv[])
{
    int  n, fd;          // n用于存储读取的字节数,fd为文件描述符
    char buf[BUFFSIZE];  // 缓冲区buf用于存储从文件读取的数据
    char line[MAXLINE];  // line用于存储从标准输入读取的文件名

    /* 从标准输入读取要cat的文件名 */
    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        if (line[strlen(line) - 1] == '\n')  // 如果读取的行末尾有换行符
            line[strlen(line) - 1] = 0;      // 将换行符替换为空字符

        /* 打开文件 */
        if ((fd = csopen(line, O_RDONLY)) < 0)  // 使用csopen函数以只读模式打开文件
            continue;                           // 如果打开失败,csopen()会打印服务器端的错误信息,然后继续读取下一个文件名

        /* 并将文件内容cat到标准输出 */
        while ((n = read(fd, buf, BUFFSIZE)) > 0)   // 从文件描述符fd指向的文件中读取数据到buf
            if (write(STDOUT_FILENO, buf, n) != n)  // 将buf中的数据写入标准输出
                err_sys("write error");             // 如果写入失败,打印错误信息
        if (n < 0)                                  // 如果读取失败
            err_sys("read error");                  // 打印错误信息
        close(fd);                                  // 关闭文件描述符
    }

    exit(0);  // 程序正常退出
}
