#include "apue.h"

int main(void)
{
    // 定义一个字符数组用于存储临时文件名,L_tmpnam定义了tmpnam函数生成的文件名的最大长度
    char name[L_tmpnam], line[MAXLINE];
    // 定义一个FILE指针,用于操作文件流
    FILE *fp;

    // 打印第一个临时文件名,tmpnam(NULL)每次调用都会生成一个新的临时文件名
    printf("%s\n", tmpnam(NULL));

    // 生成第二个临时文件名并存储在name数组中
    tmpnam(name);
    // 打印第二个临时文件名
    printf("%s\n", name);

    // 创建一个临时文件,并将文件流指针赋给fp,如果创建失败则调用err_sys函数报告错误
    if ((fp = tmpfile()) == NULL)
        err_sys("tmpfile error");
    // 向临时文件写入一行文本
    fputs("one line of output\n", fp);
    // 将文件流指针重新定位到文件开始
    rewind(fp);
    // 从临时文件读取一行文本到line数组中,如果读取失败则调用err_sys函数报告错误
    if (fgets(line, sizeof(line), fp) == NULL)
        err_sys("fgets error");
    // 将读取到的文本行打印到标准输出
    fputs(line, stdout);

    // 程序正常退出
    exit(0);
}
