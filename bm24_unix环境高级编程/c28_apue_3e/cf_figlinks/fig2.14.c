#include "apue.h"
#include <errno.h>
#include <limits.h>

// 打印系统配置信息的函数声明
static void pr_sysconf(char *, int);
// 打印路径配置信息的函数声明
static void pr_pathconf(char *, char *, int);

int main(int argc, char *argv[])
{
    // 检查命令行参数数量是否正确
    if (argc != 2)
        err_quit("usage: a.out <dirname>");

        // 检查ARG_MAX是否定义,并打印其值或提示未定义
#ifdef ARG_MAX
    printf("ARG_MAX defined to be %ld\n", (long)ARG_MAX + 0);
#else
    printf("no symbol for ARG_MAX\n");
#endif
    // 使用sysconf函数检查_ARG_MAX的值
#ifdef _SC_ARG_MAX
    pr_sysconf("ARG_MAX =", _SC_ARG_MAX);
#else
    printf("no symbol for _SC_ARG_MAX\n");
#endif

    // 类似的处理其余的系统配置符号...

    // 检查MAX_CANON是否定义,并打印其值或提示未定义
#ifdef MAX_CANON
    printf("MAX_CANON defined to be %ld\n", (long)MAX_CANON + 0);
#else
    printf("no symbol for MAX_CANON\n");
#endif
    // 使用pathconf函数检查MAX_CANON的值
#ifdef _PC_MAX_CANON
    pr_pathconf("MAX_CANON =", argv[1], _PC_MAX_CANON);
#else
    printf("no symbol for _PC_MAX_CANON\n");
#endif

    // 类似的处理其余的路径配置符号...

    // 正常退出程序
    exit(0);
}

// 打印系统配置信息的函数定义
static void
pr_sysconf(char *mesg, int name)
{
    long val;

    // 打印消息
    fputs(mesg, stdout);
    // 清除errno
    errno = 0;
    // 获取系统配置值
    if ((val = sysconf(name)) < 0)
    {
        // 如果errno不为0,检查是否是因为不支持该配置
        if (errno != 0)
        {  // 检查errno是否不为0,表示发生了错误
            if (errno == EINVAL)
            {                                         // 如果errno等于EINVAL,表示请求的参数无效或不支持
                fputs(" (not supported)\n", stdout);  // 输出提示信息到标准输出
            }
            // 其他错误,打印错误信息
            else
            {
                err_sys("sysconf error");  // 调用err_sys函数打印系统错误信息
            }
        }
        else
        {  // 如果errno为0,表示没有发生错误
            // 如果errno为0,表示没有限制
            fputs(" (no limit)\n", stdout);  // 输出提示信息到标准输出,表示没有限制
        }
    }
    else
    {
        // 打印配置值
        printf(" %ld\n", val);
    }
}

// 打印路径配置信息的函数定义
static void
pr_pathconf(char *mesg, char *path, int name)
{
    long val;

    // 打印消息
    fputs(mesg, stdout);
    // 清除errno
    errno = 0;
    // 获取路径配置值
    if ((val = pathconf(path, name)) < 0)
    {
        // 如果errno不为0,检查是否是因为不支持该配置
        // 检查errno是否不为0,如果不为0,表示发生了错误
        if (errno != 0)
        {
            // 如果errno等于EINVAL,表示请求的选项不被支持
            if (errno == EINVAL)
                fputs(" (not supported)\n", stdout);
            // 对于其他错误,打印错误信息和路径
            else
                err_sys("pathconf error, path = %s", path);
        }
        else
        {
            // 如果errno为0,表示没有发生错误,即没有限制
            fputs(" (no limit)\n", stdout);
        }
    }
    else
    {
        // 打印配置值
        printf(" %ld\n", val);
    }
}
