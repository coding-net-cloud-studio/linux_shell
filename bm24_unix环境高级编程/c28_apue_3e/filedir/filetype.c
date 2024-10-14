#include "apue.h"

// 主函数,用于检查命令行参数指定的文件类型
int main(int argc, char *argv[])
{
    int         i;    // 循环变量
    struct stat buf;  // 用于存储文件状态信息的结构体
    char       *ptr;  // 指向文件类型字符串的指针

    // 遍历命令行参数(跳过程序名本身)
    for (i = 1; i < argc; i++)
    {
        printf("%s: ", argv[i]);  // 打印文件名
        // 获取文件状态,如果出错则打印错误信息并继续下一个文件
        if (lstat(argv[i], &buf) < 0)
        {
            err_ret("lstat error");
            continue;
        }
        // 根据文件模式判断文件类型,并设置相应的字符串指针
        // 判断并打印文件类型
        if (S_ISREG(buf.st_mode))
        {
            ptr = "常规文件";  // 如果是常规文件
        }
        else if (S_ISDIR(buf.st_mode))
        {
            ptr = "目录";  // 如果是目录
        }
        else if (S_ISCHR(buf.st_mode))
        {
            ptr = "字符特殊文件";  // 如果是字符特殊文件
        }
        else if (S_ISBLK(buf.st_mode))
        {
            ptr = "块特殊文件";  // 如果是块特殊文件
        }
        else if (S_ISFIFO(buf.st_mode))
        {
            ptr = "命名管道";  // 如果是命名管道
        }
        else if (S_ISLNK(buf.st_mode))
        {
            ptr = "符号链接";  // 如果是符号链接
        }
        else if (S_ISSOCK(buf.st_mode))
        {
            ptr = "套接字";  // 如果是套接字
        }
        else
        {
            ptr = "** 未知模式 **";  // 如果文件类型未知
        }
        printf("%s\n", ptr);  // 打印文件类型
    }
    exit(0);  // 程序正常退出
}
