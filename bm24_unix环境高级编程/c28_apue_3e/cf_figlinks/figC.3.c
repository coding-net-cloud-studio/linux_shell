#include "apue.h"   // 引入apue库头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

#define DEPTH    1000                                             // 定义目录深度为1000
#define STARTDIR "/tmp"                                           // 定义起始目录为/tmp
#define NAME     "alonglonglonglonglonglonglonglonglonglongname"  // 定义一个长文件名
#define MAXSZ    (10 * 8192)                                      // 定义最大大小为80KB

int main(void)
{

    // 声明整型变量i,用于循环或其他整数操作
    int i;

    // 声明size_t类型的变量size,通常用于表示对象的大小
    size_t size;

    // 声明字符指针path,可能用于存储文件路径或其他字符串数据
    char *path;

    // 改变当前工作目录到起始目录,如果失败则输出错误信息并退出
    if (chdir(STARTDIR) < 0)
        err_sys("chdir error");

    // 循环创建目录,深度为DEPTH
    for (i = 0; i < DEPTH; i++)
    {
        // 创建目录,如果失败则输出错误信息并退出
        if (mkdir(NAME, DIR_MODE) < 0)
            err_sys("mkdir failed, i = %d", i);
        // 改变当前工作目录到新创建的目录,如果失败则输出错误信息并退出
        if (chdir(NAME) < 0)
            err_sys("chdir failed, i = %d", i);
    }

    // 创建文件afile,如果失败则输出错误信息并退出
    if (creat("afile", FILE_MODE) < 0)
        err_sys("creat error");

    /*
     * 深层目录已创建,在叶子节点有一个文件.
     * 现在让我们尝试获取它的路径名.
     */
    // 分配内存用于存储路径名
    path = path_alloc(&size);
    for (;;)
    {
        // 获取当前工作目录的绝对路径,如果成功则跳出循环
        if (getcwd(path, size) != NULL)
        {
            break;
        }
        else
        {
            // 如果失败,输出错误信息并尝试增加内存大小
            err_ret("getcwd failed, size = %ld", (long)size);
            size += 100;
            // 如果内存大小超过最大限制,则放弃并退出
            if (size > MAXSZ)
                err_quit("giving up");
            // 重新分配内存,如果失败则输出错误信息并退出
            if ((path = realloc(path, size)) == NULL)
                err_sys("realloc error");
        }
    }
    // 输出路径名的长度和内容
    printf("length = %ld\n%s\n", (long)strlen(path), path);

    // 正常退出程序
    exit(0);
}
