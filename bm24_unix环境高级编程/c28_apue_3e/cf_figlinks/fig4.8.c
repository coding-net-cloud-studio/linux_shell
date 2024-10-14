#include "apue.h"
#include <fcntl.h>

// 主函数,检查文件的可读性和打开文件的能力
int main(int argc, char *argv[])
{
    // 检查命令行参数的数量,必须为2个(程序名和文件路径)
    if (argc != 2)
        err_quit("usage: a.out <pathname>");  // 参数数量不正确,退出程序并提示用法

    // 检查文件是否可读
    if (access(argv[1], R_OK) < 0)
        err_ret("access error for %s", argv[1]);  // 文件不可读,返回错误信息
    else
        printf("read access OK\n");  // 文件可读,打印确认信息

    // 尝试以只读模式打开文件
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open error for %s", argv[1]);  // 打开文件失败,返回错误信息
    else
        printf("open for reading OK\n");  // 打开文件成功,打印确认信息

    exit(0);  // 程序正常退出
}
