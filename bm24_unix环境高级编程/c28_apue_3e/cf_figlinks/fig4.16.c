#include "apue.h"   // 引入apue库,提供系统调用和错误处理的便利函数
#include <fcntl.h>  // 引入文件控制相关的头文件

int main(void)
{
    if (open("tempfile", O_RDWR) < 0)  // 尝试以读写模式打开一个名为"tempfile"的文件
        err_sys("open error");         // 如果打开失败,调用err_sys打印错误信息并退出程序
    if (unlink("tempfile") < 0)        // 尝试删除刚才打开的文件
        err_sys("unlink error");       // 如果删除失败,调用err_sys打印错误信息并退出程序
    printf("file unlinked\n");         // 打印文件已删除的信息
    sleep(15);                         // 程序暂停15秒
    printf("done\n");                  // 暂停结束后打印完成信息
    exit(0);                           // 正常退出程序
}
