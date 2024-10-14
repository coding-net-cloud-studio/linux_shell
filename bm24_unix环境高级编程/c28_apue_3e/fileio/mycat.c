#include "apue.h"

// 定义缓冲区大小
#define BUFFSIZE 4096

/**
 * 主函数
 * 从标准输入读取数据,并将数据写入标准输出.
 * 如果读取或写入过程中发生错误,则调用err_sys函数报告错误.
 */
int main(void)
{
    int  n;              // 用于存储读取的字节数
    char buf[BUFFSIZE];  // 定义缓冲区

    // 循环读取标准输入的数据
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        // 将读取到的数据写入标准输出
        if (write(STDOUT_FILENO, buf, n) != n)
            // 如果写入的字节数与读取的字节数不一致,报告写入错误
            err_sys("write error");

    // 如果读取过程中发生错误,报告读取错误
    if (n < 0)
        err_sys("read error");

    // 正常退出程序
    exit(0);
}
