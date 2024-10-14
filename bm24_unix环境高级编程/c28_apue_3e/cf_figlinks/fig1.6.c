#include "apue.h"  // 引入apue库,提供系统调用和标准C库函数的封装

/**
 * @brief 主函数,打印当前进程ID并退出
 *
 * 该程序的主要功能是打印出当前运行的进程ID,然后正常退出.
 * 使用getpid()函数获取当前进程的ID,并使用printf()函数将其打印到标准输出.
 * 最后,调用exit(0)函数以状态码0正常退出程序.
 *
 * @return int 程序退出状态码,此处为0表示成功
 */
int main(void)
{
    printf("hello world from process ID %ld\n", (long)getpid());  // 打印进程ID
    exit(0);                                                      // 正常退出程序
}
