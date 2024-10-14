#include "apue.h"

#define BUFFSIZE 4096

// 主函数,实现从标准输入读取数据并写入到标准输出的功能
int main(void)
{
    int  n;              // 用于存储read函数的返回值,表示读取的字节数
    char buf[BUFFSIZE];  // 缓冲区,用于存储从标准输入读取的数据

    // 循环读取标准输入的数据,直到没有数据可读(read返回0)或发生错误(read返回负值)
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        // 将读取到的数据写入标准输出,如果写入的字节数与读取的字节数不一致,则输出错误信息并退出程序
        // 将缓冲区buf中的n个字节写入标准输出
        // 如果写入的字节数不等于n,则调用err_sys函数报告写入错误
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    // 如果read函数返回负值,表示读取过程中发生错误,输出错误信息并退出程序
    if (n < 0)
        err_sys("read error");

    // 正常退出程序
    exit(0);
}
