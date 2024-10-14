#include "apue.h"

// 主函数
int main(void)
{
    struct stat statbuf;  // 定义一个stat结构体用于存储文件状态信息

    // 将foo文件的set-group-ID位打开,同时关闭group-execute位
    if (stat("foo", &statbuf) < 0)                                 // 获取foo文件的状态信息
        err_sys("stat error for foo");                             // 如果出错,调用err_sys函数报告错误
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)  // 修改foo文件的权限
        err_sys("chmod error for foo");                            // 如果出错,调用err_sys函数报告错误

    // 将bar文件的权限设置为"rw-r--r--"
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  // 修改bar文件的权限
        err_sys("chmod error for bar");                           // 如果出错,调用err_sys函数报告错误

    exit(0);  // 程序正常退出
}
