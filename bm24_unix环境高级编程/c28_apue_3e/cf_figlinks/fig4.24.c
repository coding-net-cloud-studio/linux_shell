/* 引入 apue.h 头文件 */
#include "apue.h"

int main(void)
{
    /* ptr 是一个字符指针,用于存储路径名,而 size 是一个变量,用于存储路径的最大长度 */
    char  *ptr;
    size_t size;
    /* 尝试将当前目录更改为 /usr/spool/uucppublic */
    if (chdir("/usr/spool/uucppublic") < 0)
        /* 如果更改目录失败,调用自定义的错误处理函数 err_sys */
        err_sys("chdir failed");
    /* 调用自定义函数 path_alloc 获取当前路径的空间 */
    ptr = path_alloc(&size);
    /* 如果获取路径失败 */
    if (getcwd(ptr, size) == NULL)
        /* 输出错误信息并退出程序 */
        err_sys("getcwd failed");
    /* 打印当前目录 */
    printf("cwd = %s\n", ptr);
    /* 程序正常结束 */
    exit(0);
}
