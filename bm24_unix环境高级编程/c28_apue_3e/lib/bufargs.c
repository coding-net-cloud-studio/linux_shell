#include "apue.h"

#define MAXARGC 50      /* 最大参数数量在buf中 */
#define WHITE   " \t\n" /* 分隔参数的空白字符 */

/*
 * buf[] 包含以空白字符分隔的参数.我们将其转换为argv风格的指针数组,
 * 并调用用户的函数(optfunc)来处理该数组.如果解析buf出现问题,我们返回-1,
 * 否则我们返回optfunc()返回的值.注意用户的buf[]数组会被修改(每个token后放置null字符).
 */
int buf_args(char *buf, int (*optfunc)(int, char **))
{
    char *ptr, *argv[MAXARGC];  // ptr用于遍历buf,argv用于存储分割后的参数指针数组
    int   argc;                 // argc用于记录参数的数量

    if (strtok(buf, WHITE) == NULL)              /* argv[0]是必需的 */
        return (-1);                             // 如果buf为空或者没有可分割的参数,返回-1
    argv[argc = 0] = buf;                        // 将buf的首地址赋给argv[0],并初始化argc为0
    while ((ptr = strtok(NULL, WHITE)) != NULL)  // 使用strtok函数继续分割buf中的参数
    {
        if (++argc >= MAXARGC - 1) /* -1是为了在末尾留出NULL的位置 */
            return (-1);           // 如果参数数量超过最大限制,返回-1
        argv[argc] = ptr;          // 将分割出的参数指针存入argv数组
    }
    argv[++argc] = NULL;  // 在argv数组末尾添加NULL指针,表示参数列表结束

    /*
     * 由于argv[]指针指向用户的buf[],
     * 用户的函数可以直接复制这些指针,尽管argv[]数组在返回时会消失.
     */
    return ((*optfunc)(argc, argv));  // 调用用户定义的函数处理参数,并返回其结果
}
