#include "apue.h"
#include <sys/times.h>

// 打印执行时间的函数声明
static void pr_times(clock_t, struct tms *, struct tms *);
// 执行命令并计时的函数声明
static void do_cmd(char *);

int main(int argc, char *argv[])
{
    int i;

    // 关闭标准输出的缓冲
    setbuf(stdout, NULL);
    // 遍历命令行参数,对每个参数执行do_cmd函数
    for (i = 1; i < argc; i++)
        do_cmd(argv[i]); /* once for each command-line arg */
    exit(0);
}

static void
do_cmd(char *cmd) /* 执行并计时命令 */
{
    struct tms tmsstart, tmsend;  // 定义开始和结束时的tms结构体
    clock_t    start, end;        // 定义开始和结束时的clock_t类型时间
    int        status;            // 定义命令执行状态

    printf("\ncommand: %s\n", cmd);  // 打印要执行的命令

    // 获取命令执行前的时间
    if ((start = times(&tmsstart)) == -1) /* starting values */
        err_sys("times error");

    // 执行命令
    if ((status = system(cmd)) < 0) /* execute command */
        err_sys("system() error");

    // 获取命令执行后的时间
    if ((end = times(&tmsend)) == -1) /* ending values */
        err_sys("times error");

    // 打印命令执行的时间信息
    pr_times(end - start, &tmsstart, &tmsend);
    // 打印命令的退出状态
    pr_exit(status);
}

static void
pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
    static long clktck = 0;  // 定义每秒时钟滴答数

    // 如果是第一次调用,获取每秒时钟滴答数
    if (clktck == 0) /* fetch clock ticks per second first time */
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
            err_sys("sysconf error");

    // 打印真实时间
    printf("  real:  %7.2f\n", real / (double)clktck);
    // 打印用户CPU时间
    printf("  user:  %7.2f\n",
           (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
    // 打印系统CPU时间
    printf("  sys:   %7.2f\n",
           (tmsend->tms_stime - tmsstart->tms_stime) / (double)clktck);
    // 打印子进程用户CPU时间
    printf("  child user:  %7.2f\n",
           (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    // 打印子进程系统CPU时间
    printf("  child sys:   %7.2f\n",
           (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}
