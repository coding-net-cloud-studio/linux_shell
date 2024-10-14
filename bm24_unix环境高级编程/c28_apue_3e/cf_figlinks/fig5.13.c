#include "apue.h"
#include <errno.h>

// @brief 创建一个临时目录
// @param template 指向包含文件名模板的字符串的指针,其中'X'字符会被替换
void make_temp(char *template);

int main()
{
    // 正确的方式:模板存储在数组中,因此可以修改
    char good_template[] = "/tmp/dirXXXXXX";
    // 错误的方式:模板是一个指向字符串常量的指针,尝试修改会导致未定义行为
    char *bad_template = "/tmp/dirXXXXXX";

    printf("试图创建第一个临时文件...\n");
    make_temp(good_template);  // 应该成功
    printf("试图创建第二个临时文件...\n");
    make_temp(bad_template);  // 可能失败
    exit(0);
}

/*
 * make_temp - 创建一个临时文件,并根据文件是否存在进行相应处理
 *
 * @template: 用于生成临时文件名的模板字符串
 *
 * 返回值: 无
 */
void make_temp(char *template)
{
    int         fd;    // 文件描述符
    struct stat sbuf;  // 文件状态结构体

    // 使用mkstemp创建一个唯一的临时文件,并返回文件描述符
    if ((fd = mkstemp(template)) < 0)
        err_sys("can't create temp file");  // 如果创建失败,打印错误信息
    printf("temp name = %s\n", template);   // 打印临时文件名
    close(fd);                              // 关闭文件描述符

    // 使用stat获取文件状态
    if (stat(template, &sbuf) < 0)
    {
        if (errno == ENOENT)
            printf("file doesn't exist\n");  // 如果文件不存在,打印信息
        else
            err_sys("stat failed");  // 其他错误,打印错误信息
    }
    else
    {
        printf("file exists\n");  // 文件存在,打印信息
        unlink(template);         // 删除文件
    }
}
