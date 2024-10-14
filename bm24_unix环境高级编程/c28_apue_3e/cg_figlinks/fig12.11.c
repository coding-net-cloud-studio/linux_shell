#include <limits.h>  // 包含定义各种数据类型最大最小值的头文件
#include <string.h>  // 包含字符串操作的函数原型和定义的头文件

#define MAXSTRINGSZ 4096  // 定义环境变量字符串缓冲区的最大大小

static char envbuf[MAXSTRINGSZ];  // 声明一个静态字符数组,用于存储环境变量的字符串

extern char **environ;  // 声明一个指向字符串数组的指针,该数组包含当前进程的所有环境变量

char *
getenv(const char *name)
{
    // 定义变量i用于循环,len用于存储环境变量名的长度
    int i, len;

    // 获取环境变量名的长度
    len = strlen(name);
    // 遍历环境变量数组,直到遇到NULL结束
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查当前环境变量名是否与传入的name匹配,并且后面紧跟等号
        // 比较name和环境变量environ[i]的前len个字符是否相等,并且environ[i]的第len个字符是否为'='
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            // 如果匹配成功,将等号后面的值复制到envbuf中
            // strncpy函数用于将源字符串从&environ[i][len + 1]开始的字符复制到envbuf中,最多复制MAXSTRINGSZ - 1个字符
            strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
            // 返回envbuf的地址,即返回找到的环境变量的值的地址
            return (envbuf);
        }
    }
    // 如果没有找到匹配的环境变量,返回NULL
    return (NULL);
}
