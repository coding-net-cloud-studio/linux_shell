#include "apue.h"   // 引入apue库,提供系统调用和标准C库函数的封装
#include <errno.h>  // 引入errno头文件,定义了错误码

/**
 * @brief 主函数
 * @param argc 命令行参数的个数
 * @param argv 命令行参数的数组
 * @return 返回0表示程序正常退出
 */
int main(int argc, char *argv[])
{
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));  // 输出EACCES错误对应的描述信息到标准错误流
    errno = ENOENT;                                     // 设置全局错误码为ENOENT,表示没有这样的文件或目录
    perror(argv[0]);                                    // 输出当前程序名和对应的错误描述信息到标准错误流
    exit(0);                                            // 正常退出程序
}
