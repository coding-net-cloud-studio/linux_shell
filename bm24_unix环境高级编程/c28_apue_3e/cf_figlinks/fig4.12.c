#include "apue.h"

// main函数是程序的入口点
int main(void)
{
    struct stat statbuf;  // 定义一个stat结构体用于存储文件状态信息

    /*
     * 对文件"foo"进行操作:
     * 1. 使用stat函数获取文件状态
     * 2. 使用chmod函数修改文件权限,开启set-group-ID位,关闭group-execute位
     */
    if (stat("foo", &statbuf) < 0)                                 // 获取文件"foo"的状态
        err_sys("stat error for foo");                             // 如果出错,调用err_sys打印错误信息并退出
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)  // 修改文件权限
        err_sys("chmod error for foo");                            // 如果出错,调用err_sys打印错误信息并退出

    /*
     * 对文件"bar"进行操作:
     * 使用chmod函数设置文件的绝对模式为"rw-r--r--"
     */
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  // 设置文件权限
        err_sys("chmod error for bar");                           // 如果出错,调用err_sys打印错误信息并退出

    exit(0);  // 程序正常退出
}
