#include <stdio.h>

// 在非LINUX系统上,声明sys_errlist和sys_nerr
// 以下代码是错误信息列表的定义,仅在非Linux系统下有效.
// sys_errlist 是一个指向错误信息字符串数组的指针.
// sys_nerr 是错误信息的数量.

#ifndef LINUX
// 定义一个外部字符指针数组,用于存储系统错误信息的字符串表示
extern char *sys_errlist[];

// 定义一个外部整型变量,用于存储系统错误信息的数量
extern int sys_nerr;
#endif

/**
 * @brief 获取错误码对应的错误描述字符串
 *
 * @param error 错误码
 * @return char* 返回错误描述字符串,如果错误码无效,则返回"Unknown error"
 */
char *
strerror(int error)
{
    // 定义一个静态字符数组用于存储错误描述
    static char mesg[30];

    // 如果错误码在有效范围内,则返回对应的错误描述
    if (error >= 0 && error <= sys_nerr)
        return ((char *)sys_errlist[error]);

    // 如果错误码无效,则格式化一个"Unknown error"的字符串
    sprintf(mesg, "Unknown error (%d)", error);
    return (mesg);  // 返回格式化后的错误描述
}
