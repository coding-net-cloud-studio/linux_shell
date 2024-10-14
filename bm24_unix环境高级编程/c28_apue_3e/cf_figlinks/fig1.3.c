#include "apue.h"    // 包含APUE库的头文件
#include <dirent.h>  // 包含目录操作相关的头文件

int main(int argc, char *argv[])
{
    DIR           *dp;    // 定义一个目录流指针
    struct dirent *dirp;  // 定义一个目录项结构体指针

    if (argc != 2)                             // 检查命令行参数的数量是否为2(程序名和目录名)
        err_quit("usage: ls directory_name");  // 如果不是,打印错误信息并退出

    if ((dp = opendir(argv[1])) == NULL)    // 尝试打开命令行指定的目录
        err_sys("can't open %s", argv[1]);  // 如果打开失败,打印错误信息并退出

    while ((dirp = readdir(dp)) != NULL)  // 循环读取目录中的每一项
        printf("%s\n", dirp->d_name);     // 打印目录项的名称

    closedir(dp);  // 关闭目录流
    exit(0);       // 正常退出程序
}
