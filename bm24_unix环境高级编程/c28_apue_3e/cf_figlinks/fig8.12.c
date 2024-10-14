#include "apue.h"

// @brief 在屏幕上逐字符显示字符串,每个字符之间有一个延迟
// @param str 要显示的字符串
static void charatatime(char *str);

// @brief 主函数,创建子进程并分别在父进程和子进程中调用charatatime函数
int main(void)
{
    pid_t pid;  // 进程ID

    // 使用fork创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,打印错误信息并退出
    }
    else if (pid == 0)
    {
        charatatime("output from child\n");  // 子进程执行的代码
    }
    else
    {
        charatatime("output from parent\n");  // 父进程执行的代码
    }
    exit(0);  // 退出进程
}

static void
charatatime(char *str)  // 定义charatatime函数,参数为指向字符数组的指针
{
    char *ptr;  // 定义字符指针ptr
    int   c;    // 定义整型变量c用于存储字符

    setbuf(stdout, NULL);  // 设置标准输出为无缓冲模式,使得每次putc调用都会立即输出

    // 遍历字符串,直到遇到字符串结束符'\0'
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);  // 将当前字符输出到标准输出
}
