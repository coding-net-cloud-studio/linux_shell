#include "apue.h"   // 引入APUE库的头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)  // 定义文件权限掩码

/**
 * 主函数
 */
int main(void)
{
    umask(0);                                      // 设置文件模式创建屏蔽字为0,即新创建的文件具有最大的访问权限
    if (creat("foo", RWRWRW) < 0)                  // 创建一个名为"foo"的文件,并设置其权限为RWRWRW
        err_sys("creat error for foo");            // 如果创建失败,打印错误信息
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);  // 修改文件模式创建屏蔽字,使得新创建的文件不具有组和其他用户的写权限
    if (creat("bar", RWRWRW) < 0)                  // 创建一个名为"bar"的文件,并设置其权限为RWRWRW
        err_sys("creat error for bar");            // 如果创建失败,打印错误信息
    exit(0);                                       // 正常退出程序
}
