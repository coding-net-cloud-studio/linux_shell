#include "apue.h"
#include <fcntl.h>

// 定义两个字符数组
char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;  // 文件描述符

    // 创建一个名为"file.hole"的文件,如果出错则调用err_sys函数报告错误
    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("creat error");

    // 将buf1的内容写入文件,如果写入的字节数不等于10,则调用err_sys函数报告错误
    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");
    /* offset now = 10 */  // 当前文件偏移量为10

    // 将文件偏移量设置为16384,如果出错则调用err_sys函数报告错误
    if (lseek(fd, 16384, SEEK_SET) == -1)
        err_sys("lseek error");
    /* offset now = 16384 */  // 当前文件偏移量设置为16384

    // 将buf2的内容写入文件,此时写入的位置在偏移量16384处,如果写入的字节数不等于10,则调用err_sys函数报告错误
    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");
    /* offset now = 16394 */  // 写入后文件偏移量为16394

    exit(0);  // 程序正常退出
}
