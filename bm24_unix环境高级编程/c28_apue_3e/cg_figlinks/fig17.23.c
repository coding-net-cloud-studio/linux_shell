#include "apue.h"

#define MAXARGC 50      /* max number of arguments in buf */
#define WHITE   " \t\n" /* white space for tokenizing arguments */

/*
 * buf[] contains white-space-separated arguments.  We convert it to an
 * argv-style array of pointers, and call the user's function (optfunc)
 * to process the array.  We return -1 if there's a problem parsing buf,
 * else we return whatever optfunc() returns.  Note that user's buf[]
 * array is modified (nulls placed after each token).
 */

/*
 * buf_args函数将包含空格分隔参数的字符串buf转换为argv风格的指针数组,
 * 并调用用户的函数optfunc来处理这个数组.如果解析buf时出现问题,则返回-1,
 * 否则返回optfunc()的返回值.注意,用户的buf[]数组会被修改(每个token后放置null).
 */

int buf_args(char *buf, int (*optfunc)(int, char **))
{
    char *ptr, *argv[MAXARGC];  // 定义指针ptr和argv数组,用于存储分割后的参数
    int   argc;                 // 定义argc变量,用于记录参数的数量

    if (strtok(buf, WHITE) == NULL)  // 如果buf为空或者没有可分割的token,则返回-1
        return (-1);
    argv[argc = 0] = buf;  // 将buf的起始地址赋给argv[0],并初始化argc为0

    // 循环分割buf中的token,并将每个token的地址存储到argv数组中
    while ((ptr = strtok(NULL, WHITE)) != NULL)
    {
        if (++argc >= MAXARGC - 1)  // 如果参数数量超过数组最大容量,则返回-1
            return (-1);
        argv[argc] = ptr;  // 将token的地址存储到argv数组中
    }
    argv[++argc] = NULL;  // 在argv数组末尾添加NULL,表示参数列表结束

    /*
     * Since argv[] pointers point into the user's buf[],
     * user's function can just copy the pointers, even
     * though argv[] array will disappear on return.
     */

    /*
     * 由于argv[]指针指向用户buf[]数组的内存,
     * 用户的函数可以直接复制这些指针,尽管argv[]数组在返回时会消失.
     */
    return ((*optfunc)(argc, argv));  // 调用用户的optfunc函数,并返回其结果
}
