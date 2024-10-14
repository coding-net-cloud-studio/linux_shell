#include "apue.h"

#define BSZ 48  // 定义常量BSZ,表示缓冲区大小为48

// 主函数
int main()
{
    FILE *fp;        // 文件指针
    char  buf[BSZ];  // 缓冲区

    // 初始化缓冲区,填充字符'a',并在末尾设置空字符和'X'
    // 初始化缓冲区buf,将BSZ - 2个字符设置为'a'
    memset(buf, 'a', BSZ - 2);
    // 确保缓冲区以空字符结尾,这是C字符串的标准结束标志
    buf[BSZ - 2] = '\0';
    // 在缓冲区的最后一个位置设置一个'X'字符
    buf[BSZ - 1] = 'X';

    // 使用fmemopen创建一个内存文件流
    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
        err_sys("fmemopen failed");  // 如果失败,打印错误信息并退出

    // 打印初始缓冲区内容
    printf("initial buffer contents: %s\n", buf);

    // 向内存文件流写入字符串
    fprintf(fp, "hello, world");

    // 打印刷新前的缓冲区内容
    printf("before flush: %s\n", buf);

    // 刷新缓冲区,确保所有数据写入
    fflush(fp);

    // 打印刷新后的缓冲区内容
    printf("after fflush: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    // 重新初始化缓冲区,填充字符'b',并在末尾设置空字符和'X'
    // 初始化缓冲区buf,将BSZ-2个字符设置为'c'
    memset(buf, 'c', BSZ - 2);
    // 确保缓冲区以空字符结尾,这是C字符串的标准结束标志
    buf[BSZ - 2] = '\0';
    // 在缓冲区的最后一个位置设置字符'X',这可能是用于标记或其他目的
    buf[BSZ - 1] = 'X';

    // 再次向内存文件流写入字符串
    fprintf(fp, "hello, world");

    // 将文件指针重置到文件开头
    fseek(fp, 0, SEEK_SET);

    // 打印重置后的缓冲区内容
    printf("after fseek: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    // 重新初始化缓冲区,填充字符'c',并在末尾设置空字符和'X'
    // 初始化缓冲区buf,将BSZ-2个字符设置为'c'
    memset(buf, 'c', BSZ - 2);
    // 确保缓冲区以空字符结尾,这是C字符串的标准结束标志
    buf[BSZ - 2] = '\0';
    // 在缓冲区的最后一个位置设置字符'X',这可能是用于标记或其他目的
    buf[BSZ - 1] = 'X';

    // 再次向内存文件流写入字符串
    fprintf(fp, "hello, world");

    // 关闭内存文件流
    fclose(fp);

    // 打印关闭后的缓冲区内容
    printf("after fclose: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    return (0);  // 返回0表示程序成功执行
}
