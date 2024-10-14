#include <signal.h>
#include <errno.h>

/*
 * <signal.h> 通常定义 NSIG 以包括信号编号 0.
 */
#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)

/**
 * @brief 向信号集中添加一个信号
 *
 * @param set 指向信号集的指针
 * @param signo 要添加的信号编号
 * @return int 如果成功则返回 0,否则返回 -1 并设置 errno
 */
int sigaddset(sigset_t *set, int signo)
{
    if (SIGBAD(signo))  // 检查信号编号是否有效
    {
        errno = EINVAL;  // 设置错误码
        return (-1);     // 返回失败
    }
    *set |= 1 << (signo - 1);  // 将信号对应的位设置为 1
    return (0);                // 返回成功
}

/**
 * @brief 从信号集中删除一个信号
 *
 * @param set 指向信号集的指针
 * @param signo 要删除的信号编号
 * @return int 如果成功则返回 0,否则返回 -1 并设置 errno
 */
int sigdelset(sigset_t *set, int signo)
{
    if (SIGBAD(signo))  // 检查信号编号是否有效
    {
        errno = EINVAL;  // 设置错误码
        return (-1);     // 返回失败
    }
    *set &= ~(1 << (signo - 1));  // 将信号对应的位设置为 0
    return (0);                   // 返回成功
}

/**
 * @brief 检查信号是否在信号集中
 *
 * @param set 指向信号集的指针
 * @param signo 要检查的信号编号
 * @return int 如果信号在集合中则返回非零值,否则返回 0
 */
int sigismember(const sigset_t *set, int signo)
{
    if (SIGBAD(signo))  // 检查信号编号是否有效
    {
        errno = EINVAL;  // 设置错误码
        return (-1);     // 返回失败
    }
    return ((*set & (1 << (signo - 1))) != 0);  // 返回信号是否在集合中的结果
}
