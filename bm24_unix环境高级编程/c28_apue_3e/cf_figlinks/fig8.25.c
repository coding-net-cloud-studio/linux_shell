#include "apue.h"

// main函数用于打印当前进程的真实用户ID和有效用户ID
int main(void)
{
    // 打印当前进程的真实用户ID和有效用户ID
    // getuid() 函数用于获取当前进程的真实用户ID
    // geteuid() 函数用于获取当前进程的有效用户ID
    printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());

    // 退出程序,返回状态码0表示成功
    exit(0);
}
