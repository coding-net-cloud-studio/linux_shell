#include "apue.h"

// 定义charatatime函数,用于逐字符输出字符串
/**
 * @brief 逐字符输出字符串
 * @param str 要输出的字符串
 */
static void charatatime(char *);

int main(void)
{
    pid_t pid;

    // 初始化父子进程同步信号
    TELL_WAIT();

    // 创建子进程
    if ((pid = fork()) < 0)
    {
        // 如果fork失败,输出错误信息并退出
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        // 子进程执行的代码
        WAIT_PARENT();                       // 等待父进程先执行
        charatatime("output from child\n");  // 输出子进程的信息
    }
    else
    {
        // 父进程执行的代码
        charatatime("output from parent\n");  // 输出父进程的信息
        TELL_CHILD(pid);                      // 通知子进程可以开始执行
    }
    exit(0);  // 退出进程
}

// 实现charatatime函数
static void
charatatime(char *str)
{
    char *ptr;
    int   c;

    setbuf(stdout, NULL);  // 设置stdout为无缓冲模式
    // 遍历字符串并逐字符输出
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);
}
