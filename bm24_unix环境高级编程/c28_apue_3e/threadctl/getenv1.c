#include <limits.h>  // 包含整数类型的极限值定义
#include <string.h>  // 包含字符串操作的函数定义

#define MAXSTRINGSZ 4096

// 定义一个静态字符数组用于存储环境变量的值
static char envbuf[MAXSTRINGSZ];

// 引入环境变量指针数组
extern char **environ;

/**
 * @brief 获取指定环境变量的值
 *
 * @param name 环境变量的名称
 * @return char* 返回环境变量的值,如果未找到则返回NULL
 */
char *
getenv(const char *name)
{
    int i, len;

    // 获取环境变量名称的长度
    len = strlen(name);
    // 遍历环境变量数组
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查当前环境变量是否以指定名称开头并且后面紧跟等号
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            // 将环境变量的值复制到缓冲区
            strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
            // 返回缓冲区的地址
            return (envbuf);
        }
    }
    // 如果没有找到指定的环境变量,则返回NULL
    return (NULL);
}
