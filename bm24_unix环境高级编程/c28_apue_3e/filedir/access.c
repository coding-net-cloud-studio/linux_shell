#include "apue.h"   // 包含APUE库的头文件
#include <fcntl.h>  // 包含文件控制操作的头文件

int main(int argc, char *argv[])  // 主函数,接收命令行参数
{
    if (argc != 2)                            // 检查命令行参数的数量是否为2(程序名和文件路径)
        err_quit("usage: a.out <pathname>");  // 如果不是,打印使用方法并退出程序

    // 检查文件是否有读权限
    if (access(argv[1], R_OK) < 0)
        err_ret("access error for %s", argv[1]);  // 如果没有读权限,打印错误信息
    else
        printf("read access OK\n");  // 如果有读权限,打印确认信息

    // 尝试以只读模式打开文件
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open error for %s", argv[1]);  // 如果打开失败,打印错误信息
    else
        printf("open for reading OK\n");  // 如果打开成功,打印确认信息

    exit(0);  // 程序正常退出
}
