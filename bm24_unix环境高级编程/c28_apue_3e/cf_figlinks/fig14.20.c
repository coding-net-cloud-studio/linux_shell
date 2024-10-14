#include "apue.h"   // 引入自定义的apue库头文件
#include <ctype.h>  // 引入字符处理库,用于字符类型判断
#include <fcntl.h>  // 引入文件控制库,用于文件操作

#define BSZ 4096  // 定义缓冲区大小为4096字节

unsigned char buf[BSZ];  // 定义一个无符号字符数组作为缓冲区,用于存储读取的数据

// 将字符c进行ROT13加密
// @param c 需要加密的字符
// @return 加密后的字符
unsigned char
translate(unsigned char c)
{
    // 判断字符是否为字母
    if (isalpha(c))
    {
        // 如果字符在'n'到'z'之间,减去13
        if (c >= 'n')
            c -= 13;
        // 如果字符在'a'到'm'之间,加上13
        else if (c >= 'a')
            c += 13;
        // 如果字符在'N'到'Z'之间,减去13
        else if (c >= 'N')
            c -= 13;
        // 如果字符在'A'到'M'之间,加上13
        else
            c += 13;
    }
    // 返回加密后的字符
    return (c);
}

int main(int argc, char *argv[])
{
    int ifd, ofd, i, n, nw;  // 定义文件描述符和变量

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: rot13 infile outfile");

    // 打开源文件进行读取
    if ((ifd = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't open %s", argv[1]);

    // 创建或打开目标文件进行读写
    // 打开文件,如果不存在则创建,并截断现有文件
    // 参数argv[2]是要打开或创建的文件名
    // O_RDWR | O_CREAT | O_TRUNC标志表示以读写模式打开文件,如果不存在则创建文件,并截断现有文件
    // FILE_MODE是文件的权限模式
    // 如果打开文件失败,err_sys函数会打印错误信息并退出程序
    if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("can't create %s", argv[2]);

    // 循环读取源文件内容
    while ((n = read(ifd, buf, BSZ)) > 0)
    {
        // 对读取的每个字符进行转换
        // 以下代码段遍历数组buf中的每个元素,并使用translate函数进行转换.
        // 假设translate函数已经定义,并能正确处理buf中的每个元素.
        for (i = 0; i < n; i++)
        {
            // 对buf中的第i个元素调用translate函数,并将结果赋值回原位置
            buf[i] = translate(buf[i]);
        }

        // 将转换后的字符写入目标文件
        // 尝试将buf中的n个字节写入ofd文件描述符
        if ((nw = write(ofd, buf, n)) != n)
        {
            // 如果写入的字节数nw小于0,表示写入失败
            if (nw < 0)
                err_sys("write failed");  // 输出系统错误信息并退出
            else
                // 如果写入的字节数nw小于请求的字节数n,表示写入不完整
                err_quit("short write (%d/%d)", nw, n);  // 输出写入不完整的错误信息并退出
        }
    }

    // 同步目标文件到磁盘
    fsync(ofd);
    // 正常退出程序
    exit(0);
}
