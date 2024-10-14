#include "apue.h"  // 引入apue库,该库提供了一系列系统编程的工具和函数

/*
 * Used when we had planned to send an fd using send_fd(),
 * but encountered an error instead.  We send the error back
 * using the send_fd()/recv_fd() protocol.
 */

/*
 * 当我们计划使用send_fd()发送一个文件描述符时,
 * 但遇到了错误,我们使用send_fd()/recv_fd()协议将错误发送回去.
 */

/**
 * @brief 向文件描述符发送错误代码和消息
 *
 * 该函数用于向指定的文件描述符发送一个错误代码以及相关的错误消息.
 *
 * @param fd 文件描述符,用于发送错误信息的通道
 * @param errcode 错误代码,表示发生的错误类型
 * @param msg 指向错误消息字符串的指针
 * @return int 返回值表示操作的成功与否,成功通常返回0,失败返回非0值
 */
int send_err(int fd, int errcode, const char *msg)
{
    int n;

    // 如果消息不为空,则发送错误消息

    // 获取msg字符串的长度
    if ((n = strlen(msg)) > 0)
        // 如果长度大于0,则尝试通过文件描述符fd发送msg
        if (writen(fd, msg, n) != n) /* 发送错误消息 */
            // 如果发送的字节数与msg长度不一致,说明发送出错
            return (-1);  // 返回错误码

    // 确保错误码是负数
    if (errcode >= 0)
        errcode = -1; /* 必须是负数 */

    // 发送错误码
    if (send_fd(fd, errcode) < 0)
        return (-1);

    // 发送成功
    return (0);
}
