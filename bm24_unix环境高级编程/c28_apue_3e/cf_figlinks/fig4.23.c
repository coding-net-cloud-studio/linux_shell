#include "apue.h"

// main函数是程序的入口点
int main(void)
{
    // 尝试改变当前工作目录到/tmp
    if (chdir("/tmp") < 0)
        // 如果失败,使用err_sys打印错误信息并退出程序
        err_sys("chdir failed");
    // 如果成功,打印成功信息
    printf("chdir to /tmp succeeded\n");
    // 正常退出程序
    exit(0);
}
