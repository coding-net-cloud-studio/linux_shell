#include "apue.h"

// 主函数
int main(void)
{
    char  *ptr;   // 定义一个字符指针用于存储路径
    size_t size;  // 定义一个size_t类型的变量用于存储路径的大小

    // 尝试切换到目录/usr/spool/uucppublic,如果失败则调用err_sys函数报告错误
    if (chdir("/usr/spool/uucppublic") < 0)
        err_sys("chdir failed");

    // 调用path_alloc函数分配内存空间,并将所需大小存储在size变量中
    ptr = path_alloc(&size); /* our own function */

    // 获取当前工作目录的绝对路径,如果失败则调用err_sys函数报告错误
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");

    // 打印当前工作目录
    printf("cwd = %s\n", ptr);

    // 退出程序
    exit(0);
}
