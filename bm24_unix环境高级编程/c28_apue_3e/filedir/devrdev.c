#include "apue.h"
/* # wmtag_memo_我修改了这里_开始 */
#include <sys/sysmacros.h>  // 包含sysmacros.h头文件,用于提供major和minor函数的声明
/* # wmtag_memo_我修改了这里_结束 */
#ifdef SOLARIS
#include <sys/mkdev.h>  // 如果定义了SOLARIS,则包含mkdev.h头文件
#endif

/**
 * @brief 主函数,用于打印文件或设备的设备号
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    int         i;    // 循环变量
    struct stat buf;  // 用于存储stat函数获取的文件状态信息

    for (i = 1; i < argc; i++)  // 遍历命令行参数
    {
        printf("%s: ", argv[i]);      // 打印文件或设备的名称
        if (stat(argv[i], &buf) < 0)  // 获取文件或设备的状态信息
        {
            err_ret("stat error");  // 如果出错,打印错误信息并继续下一个循环
            continue;
        }

        printf("dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));  // 打印主设备号和次设备号

        // 如果是字符设备或块设备,打印设备类型和rdev的主设备号和次设备号
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode))
        {
            printf(" (%s) rdev = %d/%d",
                   (S_ISCHR(buf.st_mode)) ? "character" : "block",
                   major(buf.st_rdev),
                   minor(buf.st_rdev));
        }
        printf("\n");  // 换行
    }

    exit(0);  // 正常退出程序
}
