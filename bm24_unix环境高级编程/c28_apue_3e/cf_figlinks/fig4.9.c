#include "apue.h"   // 包含APUE库的头文件
#include <fcntl.h>  // 包含文件控制操作的头文件

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)  // 定义文件权限掩码

/**
 * main - 主函数
 *
 * 返回: 成功返回0,失败返回错误码
 */
int main(void)
{
    umask(0);                                      // 设置文件模式创建掩码为0,允许所有权限
    if (creat("foo", RWRWRW) < 0)                  // 创建文件foo,并设置权限为RWRWRW
        err_sys("creat error for foo");            // 如果创建失败,打印错误信息
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);  // 恢复文件模式创建掩码,限制组和其他用户的写权限
    if (creat("bar", RWRWRW) < 0)                  // 创建文件bar,并尝试设置权限为RWRWRW
        err_sys("creat error for bar");            // 如果创建失败,打印错误信息
    exit(0);                                       // 正常退出程序
}
