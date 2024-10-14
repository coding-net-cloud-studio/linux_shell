#include "apue.h"

// 定义缓冲区大小为48字节
#define BSZ 48

// 主函数
int main()
{
    FILE *fp;        // 文件指针
    char  buf[BSZ];  // 缓冲区

    // 初始化缓冲区,填充字符'a',并在末尾设置'\0'和'X'
    memset(buf, 'a', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    // 使用fmemopen创建一个流,指向缓冲区buf
    if ((fp = fmemopen(buf, BSZ, "w+")) == NULL)
        err_sys("fmemopen failed");  // 如果失败,打印错误信息

    // 打印初始缓冲区内容
    printf("initial buffer contents: %s\n", buf);

    // 向流中写入字符串
    fprintf(fp, "hello, world");

    // 打印刷新前的缓冲区内容
    printf("before flush: %s\n", buf);

    // 刷新流,确保所有数据都写入到缓冲区
    fflush(fp);

    // 打印刷新后的缓冲区内容
    printf("after fflush: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    // 重新初始化缓冲区,填充字符'b'
    memset(buf, 'b', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    // 再次向流中写入字符串
    fprintf(fp, "hello, world");

    // 将文件指针移动到流的开始位置
    fseek(fp, 0, SEEK_SET);

    // 打印移动指针后的缓冲区内容
    printf("after  fseek: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    // 重新初始化缓冲区,填充字符'c'
    memset(buf, 'c', BSZ - 2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    // 再次向流中写入字符串
    fprintf(fp, "hello, world");

    // 关闭流
    fclose(fp);

    // 打印关闭流后的缓冲区内容
    printf("after fclose: %s\n", buf);

    // 打印缓冲区中字符串的长度
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    return (0);  // 返回0表示程序正常结束
}
