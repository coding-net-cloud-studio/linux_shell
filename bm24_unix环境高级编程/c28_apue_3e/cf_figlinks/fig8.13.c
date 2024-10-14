#include "apue.h"

// 定义charatatime函数,该函数用于逐字符输出字符串
static void charatatime(char *);

int main(void)
{
    pid_t pid;  // 定义进程ID变量

    TELL_WAIT();  // 通知父进程和子进程开始等待

    // 使用fork创建子进程
    if ((pid = fork()) < 0)
    {
        err_sys("fork error");  // 如果fork失败,输出错误信息
    }
    else if (pid == 0)
    {
        WAIT_PARENT();                       // 子进程等待父进程先执行
        charatatime("output from child\n");  // 子进程输出字符串
    }
    else
    {
        charatatime("output from parent\n");  // 父进程输出字符串
        TELL_CHILD(pid);                      // 父进程通知子进程继续执行
    }
    exit(0);  // 退出进程
}

static void
charatatime(char *str)  // 定义charatatime函数,参数为指向字符数组的指针
{
    char *ptr;  // 定义字符指针ptr
    int   c;    // 定义整型变量c,用于存储从字符串中读取的字符

    setbuf(stdout, NULL);  // 设置stdout为无缓冲模式,使得每次putc函数调用都会立即输出字符

    // 遍历字符串,直到遇到字符串结束符'\0'
    for (ptr = str; (c = *ptr++) != 0;)
        putc(c, stdout);  // 将当前字符输出到stdout
}
