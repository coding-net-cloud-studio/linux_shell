#include "apue.h"

// 主函数
int main(void)
{
    FILE *fp;  // 文件指针,用于打开和写入文件
    char *p;   // 指向获取到的登录名的指针

    daemonize("getlog");                 // 将程序转为守护进程运行
    p  = getlogin();                     // 获取当前登录名
    fp = fopen("/tmp/getlog.out", "w");  // 打开文件准备写入
    if (fp != NULL)                      // 检查文件是否成功打开
    {
        if (p == NULL)                           // 如果获取登录名失败
            fprintf(fp, "no login name\n");      // 写入文件表示没有登录名
        else                                     // 如果获取登录名成功
            fprintf(fp, "login name: %s\n", p);  // 写入文件显示登录名
    }
    exit(0);  // 程序正常退出
}
