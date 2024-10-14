#include "apue.h"

// sleep2函数声明,模拟sleep函数,返回未睡眠的秒数
/**
 * @brief 模拟sleep函数,返回未睡眠的秒数
 *
 * @param seconds 需要睡眠的秒数
 * @return unsigned int 未睡眠的秒数
 */
unsigned int sleep2(unsigned int);

// sig_int函数声明,用于处理SIGINT信号
/**
 * @brief 处理SIGINT信号的函数
 *
 * @param signo 信号编号
 */
static void sig_int(int);

int main(void)
{
    unsigned int unslept;

    // 注册SIGINT信号的处理函数为sig_int
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    // 调用sleep2函数,尝试睡眠5秒
    unslept = sleep2(5);

    // 打印未睡眠的秒数
    printf("sleep2 returned: %u\n", unslept);

    // 退出程序
    exit(0);
}
static void
sig_int(int signo)  // 定义信号处理函数sig_int,参数为接收到的信号编号
{
    int          i, j;  // 定义整型变量i和j,用于循环计数
    volatile int k;     // 定义易失性整型变量k,确保编译器不会优化掉对k的操作

    /*
     * Tune these loops to run for more than 5 seconds
     * on whatever system this test program is run.
     */
    printf("\nsig_int starting\n");  // 打印信号处理函数开始的提示信息

    // 外层循环,循环300000次
    for (i = 0; i < 300000; i++)
        // 内层循环,每次外层循环都会执行4000次
        for (j = 0; j < 4000; j++)
            k += i * j;  // 计算i和j的乘积,并累加到k上,由于k是易失性变量,每次累加都会被立即写入内存

    printf("sig_int finished\n");  // 打印信号处理函数结束的提示信息
}
