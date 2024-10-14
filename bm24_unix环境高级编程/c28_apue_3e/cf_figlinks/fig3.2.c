#include "apue.h"
#include <fcntl.h>

// 定义两个字符数组,用于测试文件空洞
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
    /* offset now = 10 */

    // 将文件指针移动到16384的位置,如果出错则调用err_sys函数报告错误
    if (lseek(fd, 16384, SEEK_SET) == -1)
        err_sys("lseek error");
    /* offset now = 16384 */

    // 在当前文件指针位置写入buf2的内容,如果写入的字节数不等于10,则调用err_sys函数报告错误
    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");
    /* offset now = 16394 */

    // 正常退出程序
    exit(0);
}
