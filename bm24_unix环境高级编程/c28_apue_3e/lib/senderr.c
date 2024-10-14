#include "apue.h"

/*
 * 当我们计划使用send_fd()发送一个文件描述符时,
 * 但遇到了错误,我们使用send_fd()/recv_fd()协议将错误信息发送回去.
 *
 * @param fd 文件描述符
 * @param errcode 错误代码
 * @param msg 错误消息
 * @return 成功返回0,失败返回-1
 */
int send_err(int fd, int errcode, const char *msg)
{
    int n;

    // 如果消息不为空,则发送错误消息
    if ((n = strlen(msg)) > 0)
        if (writen(fd, msg, n) != n) /* 发送错误消息 */
            return (-1);

    // 确保错误代码是负数
    if (errcode >= 0)
        errcode = -1;

    // 发送错误代码
    if (send_fd(fd, errcode) < 0)
        return (-1);

    return (0);
}
