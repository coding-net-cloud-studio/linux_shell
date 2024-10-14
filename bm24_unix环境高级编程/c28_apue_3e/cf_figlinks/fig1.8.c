#include "apue.h"   // 引入apue库,提供系统调用和标准C库函数的封装
#include <errno.h>  // 引入错误处理头文件,定义了错误码

/**
 * @brief 主函数
 * @param argc 命令行参数的个数
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));  // 输出EACCES错误码对应的错误信息到标准错误流
    errno = ENOENT;                                     // 设置全局错误变量errno为ENOENT,表示没有这样的文件或目录
    perror(argv[0]);                                    // 输出当前程序名和errno对应的错误信息到标准错误流
    exit(0);                                            // 正常退出程序,退出状态码为0
}
