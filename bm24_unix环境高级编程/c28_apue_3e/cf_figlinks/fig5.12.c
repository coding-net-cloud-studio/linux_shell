// 引入 apue.h 头文件,该头文件包含了一些用于系统编程的函数原型和宏定义
#include "apue.h"

/**
 * 主函数,测试临时文件名生成函数和临时文件创建函数
 */
int main(void)
{
    char  name[L_tmpnam], line[MAXLINE];  // 定义两个字符数组,用于存储临时文件名和读取的文件内容
    FILE *fp;                             // 定义一个文件指针,用于指向临时文件

    printf("%s\n", tmpnam(NULL));  // 打印第一个临时文件名,tmpnam函数生成一个唯一的临时文件名

    tmpnam(name);          // 生成第二个临时文件名,并存储在 name 数组中,同样使用tmpnam函数
    printf("%s\n", name);  // 打印第二个临时文件名

    // 创建一个临时文件,如果创建失败则调用 err_sys 函数输出错误信息并退出程序
    // tmpfile函数创建一个临时文件,并返回一个文件指针,该文件在关闭后会被自动删除
    if ((fp = tmpfile()) == NULL)
        err_sys("tmpfile error");

    // 向临时文件写入一行内容,fputs函数将字符串写入文件流
    fputs("one line of output\n", fp);

    // 将文件指针移动到文件开头,rewind函数用于重置文件指针到文件的开始位置
    rewind(fp);

    // 从临时文件读取一行内容到 line 数组中,如果读取失败则调用 err_sys 函数输出错误信息并退出程序
    // fgets函数从文件流中读取一行数据
    if (fgets(line, sizeof(line), fp) == NULL)
        err_sys("fgets error");

    // 将读取到的内容输出到标准输出,fputs函数将字符串输出到指定的文件流,这里是标准输出(stdout)
    fputs(line, stdout);

    // 程序正常退出,exit函数用于终止程序执行
    exit(0);
}
