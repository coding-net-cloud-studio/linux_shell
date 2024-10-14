#include "apue.h"  // 引入apue库,提供系统错误处理的函数
#include <pwd.h>   // 引入passwd结构体和相关的函数,用于获取用户信息

int main(void) /* FreeBSD/Mac OS X版本 */
{
    struct passwd *ptr;  // 定义一个指向passwd结构体的指针

    // 使用getpwnam函数尝试获取名为"sar"的用户信息
    if ((ptr = getpwnam("sar")) == NULL)
        err_sys("getpwnam error");  // 如果获取失败,调用err_sys打印错误信息并退出程序

    // 打印用户密码字段,如果密码字段为空或者为0,则打印"(null)"
    printf("pw_passwd = %s\n", ptr->pw_passwd == NULL || ptr->pw_passwd[0] == 0 ? "(null)" : ptr->pw_passwd);

    exit(0);  // 程序正常退出
}
