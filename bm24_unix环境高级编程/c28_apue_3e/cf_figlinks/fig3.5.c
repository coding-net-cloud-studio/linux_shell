#include "apue.h"

#define BUFFSIZE 4096

// <summary>
// 主函数,用于从标准输入读取数据并写入到标准输出.
// </summary>
int main(void)
{
    int  n;              // 用于存储读取的字节数
    char buf[BUFFSIZE];  // 缓冲区,用于存储从标准输入读取的数据

    // <summary>
    // 循环读取标准输入的数据,直到没有数据可读.
    // </summary>
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
    {
        // <summary>
        // 将读取到的数据写入标准输出.
        // 如果写入的字节数与读取的字节数不一致,则输出错误信息并退出程序.
        // </summary>
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    }

    // <summary>
    // 如果读取操作返回负值,表示发生了错误,输出错误信息并退出程序.
    // </summary>
    if (n < 0)
        err_sys("read error");

    // <summary>
    // 正常退出程序.
    // </summary>
    exit(0);
}
