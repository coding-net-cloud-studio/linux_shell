#include "apue.h"

/**
 * @brief 程序的入口点
 *
 * 这个程序会将当前用户的登录名写入到 /tmp/getlog.out 文件中.如果无法获取登录名,则会写入 "no login name".
 *
 * @return int 程序的退出状态码
 */
int main(void)
{

    // 声明一个文件指针fp,用于指向文件
    FILE *fp;

    // 声明一个字符指针p,用于指向字符数据
    char *p;

    // 将当前进程变为守护进程,并命名为 "getlog"
    daemonize("getlog");

    // 获取当前用户的登录名
    p = getlogin();

    // 打开 /tmp/getlog.out 文件,准备写入
    // 打开 /tmp/getlog.out 文件,准备写入
    fp = fopen("/tmp/getlog.out", "w");
    if (fp != NULL)
    {
        // 如果获取登录名失败,写入 "no login name"
        if (p == NULL)
            fprintf(fp, "no login name\n");
        // 如果获取登录名成功,写入登录名
        else
            fprintf(fp, "login name: %s\n", p);
    }

    // 关闭文件
    fclose(fp);
    // 程序正常结束,返回 0
    exit(0);
}
