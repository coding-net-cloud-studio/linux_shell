#include "apue.h"

// 主函数,程序入口
int main(void)
{
    // 尝试切换当前工作目录到 /tmp
    // 如果失败,调用 err_sys 函数输出错误信息并退出程序
    if (chdir("/tmp") < 0)
        err_sys("chdir failed");

    // 如果切换成功,打印成功信息
    printf("chdir to /tmp succeeded\n");

    // 正常退出程序
    exit(0);
}
