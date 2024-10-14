#include "apue.h"

// 定义缓冲区大小
#define BUFFSIZE 4096

/**
 * 主函数,从标准输入读取数据并写入到标准输出.
 * 使用read系统调用从STDIN_FILENO(标准输入)读取数据到缓冲区buf,
 * 然后使用write系统调用将缓冲区的内容写入到STDOUT_FILENO(标准输出).
 * 如果在读取或写入过程中发生错误,则调用err_sys打印错误信息并退出程序.
 */
int main(void)
{
    int  n;              // 用于存储read系统调用的返回值
    char buf[BUFFSIZE];  // 缓冲区,用于存储从标准输入读取的数据

    // 循环读取标准输入的数据,直到没有数据可读(n <= 0)
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        // 将读取到的数据写入标准输出,如果写入的字节数不等于读取的字节数,则表示写入错误
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");  // 写入错误时调用err_sys函数

    // 如果read系统调用返回负值,表示读取过程中发生了错误
    if (n < 0)
        err_sys("read error");  // 读取错误时调用err_sys函数

    exit(0);  // 正常退出程序
}
