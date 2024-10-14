#include "apue.h"   // 引入APUE库的头文件
#include <fcntl.h>  // 引入文件控制相关的头文件

#define DEPTH    1000 /* 目录深度 */                                       // 定义目录的最大深度
#define STARTDIR "/tmp" /* 起始目录 */                                     // 定义操作开始的目录路径
#define NAME     "alonglonglonglonglonglonglonglonglongname" /* 文件名 */  // 定义要创建的文件名
#define MAXSZ    (10 * 8192) /* 最大大小 */                                // 定义文件的最大大小
int main(void)
{
    int    i;     // 循环变量
    size_t size;  // 路径缓冲区大小
    char  *path;  // 路径缓冲区指针

    // 改变当前工作目录到起始目录
    if (chdir(STARTDIR) < 0)
        err_sys("chdir error");

    // 创建深度目录结构
    for (i = 0; i < DEPTH; i++)
    {
        // 创建目录
        if (mkdir(NAME, DIR_MODE) < 0)
            err_sys("mkdir failed, i = %d", i);
        // 改变当前工作目录到新创建的目录
        if (chdir(NAME) < 0)
            err_sys("chdir failed, i = %d", i);
    }

    // 在最深目录创建文件
    if (creat("afile", FILE_MODE) < 0)
        err_sys("creat error");

    /*
     * 深层目录已创建,叶节点处有一个文件.
     * 现在尝试获取其路径名.
     */
    // 分配初始路径缓冲区
    path = path_alloc(&size);
    for (;;)
    {
        // 获取当前工作目录的绝对路径
        if (getcwd(path, size) != NULL)
        {
            break;  // 成功获取路径,退出循环
        }
        else
        {
            // 获取路径失败,增加缓冲区大小并重试
            // 如果getcwd失败,打印错误信息并增加缓冲区大小
            err_ret("getcwd失败,大小 = %ld", (long)size);  // 打印错误信息,显示当前缓冲区大小
            size += 100;                                   // 增加缓冲区大小
            if (size > MAXSZ)                              // 检查新的缓冲区大小是否超过最大限制
                err_quit("放弃");                          // 如果超过最大限制,打印放弃信息并退出
            // 重新分配更大的缓冲区
            if ((path = realloc(path, size)) == NULL)  // 尝试重新分配内存
                err_sys("realloc错误");                // 如果重新分配失败,打印系统错误信息
        }
    }
    // 输出路径长度和路径字符串
    printf("length = %ld\n%s\n", (long)strlen(path), path);

    exit(0);  // 程序正常退出
}
