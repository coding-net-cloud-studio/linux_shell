#include "apue.h"

// main 函数打印当前进程的用户ID和组ID,然后退出
int main(void)
{
    // 使用getuid()函数获取当前进程的用户ID,并使用getgid()函数获取当前进程的组ID
    // 然后使用printf函数将它们打印到标准输出
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    // 调用exit函数,以状态码0正常退出程序
    exit(0);
}
