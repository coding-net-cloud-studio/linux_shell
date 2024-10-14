#include "apue.h"       // 包含apue.h头文件
#include <sys/times.h>  // 包含times.h头文件

static void pr_times(clock_t, struct tms *, struct tms *);  // 声明一个静态函数pr_times,用于打印程序运行时间
static void do_cmd(char *);                                 // 声明一个静态函数do_cmd,用于执行命令

int main(int argc, char *argv[])  // 主函数,接收两个参数:argc(参数个数)和argv(参数数组)
{
    int i;  // 定义一个整型变量i,用于循环计数

    setbuf(stdout, NULL);       // 将stdout设置为行缓冲,避免输出时阻塞
    for (i = 1; i < argc; i++)  // 遍历参数数组
        do_cmd(argv[i]);        /* once for each command-line arg */
    exit(0);
}
static void
do_cmd(char *cmd) /* 执行并计时命令 "cmd" */
{
    struct tms tmsstart, tmsend;  // 定义开始和结束时间结构体
    clock_t    start, end;        // 定义开始和结束的时钟时间
    int        status;            // 定义命令执行状态

    printf("\ncommand: %s\n", cmd);  // 打印要执行的命令

    if ((start = times(&tmsstart)) == -1) /* 获取开始时间 */
        err_sys("times error");           // 如果获取时间失败,打印错误并退出

    if ((status = system(cmd)) < 0) /* 执行命令 */
        err_sys("system() error");  // 如果命令执行失败,打印错误并退出

    if ((end = times(&tmsend)) == -1) /* 获取结束时间 */
        err_sys("times error");       // 如果获取时间失败,打印错误并退出

    pr_times(end - start, &tmsstart, &tmsend);  // 打印命令执行的时间信息
    pr_exit(status);                            // 打印命令的执行状态
}

static void
pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
    // 静态变量,用于存储每秒的时钟滴答数
    static long clktck = 0;

    // 如果clk_tck为0,则首次获取每秒的时钟滴答数
    if (clktck == 0)
        // 使用sysconf函数获取_SC_CLK_TCK的值,即每秒的时钟滴答数
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
            // 如果获取失败,打印错误信息并退出
            err_sys("sysconf error");

    // 打印实际经过的时间(秒)
    printf("  real:  %7.2f\n", real / (double)clktck);
    // 打印用户态时间(秒)
    printf("  user:  %7.2f\n",
           (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
    // 打印系统态时间(秒)
    printf("  sys:   %7.2f\n",
           (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);
    // 打印子进程用户态时间(秒)
    printf("  child user:  %7.2f\n",
           (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    // 打印子进程系统态时间(秒)
    printf("  child sys:   %7.2f\n",
           (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}
