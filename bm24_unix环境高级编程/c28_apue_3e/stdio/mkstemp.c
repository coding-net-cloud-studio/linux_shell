#include "apue.h"
#include <errno.h>

// 函数功能:创建一个临时文件,并根据传入的模板字符串生成唯一的文件名
// 参数:template - 用于生成临时文件名的模板字符串
void make_temp(char *template);

int main()
{
    // 正确的模板字符串,包含文件路径和6个X作为占位符
    char good_template[] = "/tmp/dirXXXXXX";
    // 错误的模板字符串,指向同一个字符串对象,可能导致竞争条件
    char *bad_template = "/tmp/dirXXXXXX";

    printf("trying to create first temp file...\n");
    make_temp(good_template);  // 尝试创建第一个临时文件
    printf("trying to create second temp file...\n");
    make_temp(bad_template);  // 尝试创建第二个临时文件
    exit(0);
}

void make_temp(char *template)
{
    int         fd;    // 文件描述符
    struct stat sbuf;  // 文件状态结构体

    // 使用mkstemp函数创建临时文件,并返回文件描述符
    // 如果失败,打印错误信息并退出
    if ((fd = mkstemp(template)) < 0)
        err_sys("can't create temp file");
    printf("temp name = %s\n", template);  // 打印创建的临时文件名
    close(fd);                             // 关闭文件描述符

    // 使用stat函数检查文件是否存在
    if (stat(template, &sbuf) < 0)
    {
        // 如果errno为ENOENT,表示文件不存在
        if (errno == ENOENT)
            printf("file doesn't exist\n");
        else
            // 其他错误,打印错误信息
            err_sys("stat failed");
    }
    else
    {
        // 文件存在,打印信息并删除文件
        printf("file exists\n");
        unlink(template);
    }
}
