#include "apue.h"    // 引入APUE库头文件
#include <shadow.h>  // 引入shadow库头文件,用于访问shadow密码文件

/**
 * @brief 主函数,获取指定用户名的shadow密码信息
 *
 * @return int 程序执行状态码,0表示成功
 */
int main(void) /* Linux/Solaris version */
{
    struct spwd *ptr;  // 定义一个指向spwd结构体的指针,用于存储获取到的shadow密码信息

    // 使用getspnam函数尝试获取用户名为"sar"的shadow密码信息
    if ((ptr = getspnam("sar")) == NULL)
        err_sys("getspnam error");  // 如果获取失败,调用err_sys函数输出错误信息并退出程序

    // 打印获取到的shadow密码信息,如果sp_pwdp字段为空,则打印"(null)"
    printf("sp_pwdp = %s\n", ptr->sp_pwdp == NULL || ptr->sp_pwdp[0] == 0 ? "(null)" : ptr->sp_pwdp);

    exit(0);  // 程序执行成功,退出并返回状态码0
}
