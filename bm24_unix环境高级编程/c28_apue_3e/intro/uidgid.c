#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

// 主函数,打印当前进程的用户ID和组ID
int main(void)
{
    // 使用getuid()函数获取当前进程的用户ID,getgid()函数获取当前进程的组ID
    // 然后使用printf函数打印这两个ID
    // 打印当前进程的用户ID和组ID
    // 使用getuid()函数获取当前进程的用户ID
    // 使用getgid()函数获取当前进程的组ID
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    // 程序正常结束,调用exit(0)函数
    exit(0);
}
