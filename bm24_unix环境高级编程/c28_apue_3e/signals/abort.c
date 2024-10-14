#include <signal.h>  // 包含信号处理函数的头文件
#include <stdio.h>   // 包含标准输入输出函数的头文件
#include <stdlib.h>  // 包含标准库函数的头文件
#include <unistd.h>  // 包含Unix系统调用函数的头文件

// @brief POSIX风格的abort()函数,用于异常退出程序
void abort(void)
{
    sigset_t         mask;    // 信号集,用于管理信号屏蔽
    struct sigaction action;  // 信号动作结构体,用于设置信号处理函数

    /*
     * 确保SIGABRT信号不会被忽略.
     * 如果当前的信号处理函数是SIG_IGN(忽略),则将其重置为默认处理函数SIG_DFL
     */
    sigaction(SIGABRT, NULL, &action);
    // 如果信号处理函数被设置为SIG_IGN,即忽略该信号
    if (action.sa_handler == SIG_IGN)
    {
        // 将信号处理函数重置为默认行为
        action.sa_handler = SIG_DFL;
        // 使用新的信号处理设置更新SIGABRT信号的处理方式
        sigaction(SIGABRT, &action, NULL);
    }

    /* 如果SIGABRT信号的处理函数是默认的SIG_DFL,则刷新所有打开的标准I/O流 */
    if (action.sa_handler == SIG_DFL)
        fflush(NULL);

    /*
     * 确保SIGABRT信号不会被阻塞.
     * 创建一个信号集mask,其中除SIGABRT外所有信号都被屏蔽
     */
    // 初始化信号集,将所有信号添加到集合中
    sigfillset(&mask);

    // 从信号集中删除SIGABRT信号,这样程序在接收到SIGABRT信号时不会终止
    sigdelset(&mask, SIGABRT);

    // 设置进程的信号屏蔽字,使用新的信号集mask,旧的信号集不保存
    sigprocmask(SIG_SETMASK, &mask, NULL);

    /* 向当前进程发送SIGABRT信号,导致程序异常退出 */
    kill(getpid(), SIGABRT);

    /*
     * 如果程序执行到这里,说明进程捕获了SIGABRT信号并返回了.
     * 再次刷新所有打开的标准I/O流,并尝试重置SIGABRT信号的处理函数和屏蔽状态
     */
    // 重置标准输出流的缓冲区,确保所有数据都被写入到文件或设备
    fflush(NULL);

    // 设置SIGABRT信号的处理函数为默认处理函数
    // SIGABRT信号通常在程序异常终止时产生,如调用abort()函数
    // SIG_DFL表示使用系统默认的处理方式,通常是终止程序并生成核心转储
    action.sa_handler = SIG_DFL;

    // 修改SIGABRT信号的处理动作
    // 第一个参数指定信号编号,第二个参数指向新的处理动作结构体,第三个参数用于保存原有处理动作结构体(这里不需要)
    sigaction(SIGABRT, &action, NULL);

    // 设置进程的信号掩码,屏蔽mask中包含的信号
    // SIG_SETMASK表示设置信号掩码,&mask是指向新掩码的指针,NULL表示不保存旧的掩码
    sigprocmask(SIG_SETMASK, &mask, NULL);

    /* 再次发送SIGABRT信号 */
    kill(getpid(), SIGABRT);

    /* 这个exit调用理论上不应该被执行到 */
    exit(1);
}
