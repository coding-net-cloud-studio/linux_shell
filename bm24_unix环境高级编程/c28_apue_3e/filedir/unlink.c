#include "apue.h"   // 引入APUE库头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

/**
 * @brief 主函数
 *
 * 本程序尝试打开一个名为tempfile的文件,然后立即删除它.程序会等待15秒后结束.
 *
 * @return int 程序退出状态码
 */
int main(void)
{
    // 尝试以读写模式打开文件,如果失败则输出错误信息并退出
    if (open("tempfile", O_RDWR) < 0)
        err_sys("open error");

    // 删除刚刚打开的文件,如果失败则输出错误信息并退出
    if (unlink("tempfile") < 0)
        err_sys("unlink error");

    // 输出文件已删除的信息
    printf("file unlinked\n");

    // 程序暂停15秒
    sleep(15);

    // 程序结束前输出完成信息
    printf("done\n");

    // 正常退出程序
    exit(0);
}
