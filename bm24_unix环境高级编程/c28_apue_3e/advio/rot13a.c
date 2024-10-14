#include "apue.h"   // 引入apue库,提供系统调用和标准C库函数的封装
#include <ctype.h>  // 引入字符处理函数库,如isalpha(), islower(), isupper()等
#include <fcntl.h>  // 引入文件控制函数库,如open(), close()等

#define BSZ 4096

// 定义一个缓冲区用于读写数据
unsigned char buf[BSZ];

/**
 * 将字符进行ROT13加密
 * @param c 需要加密的字符
 * @return 加密后的字符
 */
unsigned char
translate(unsigned char c)
{
    // 判断字符是否为字母
    if (isalpha(c))
    {
        // 如果是小写字母'n'到'z',则减去13
        if (c >= 'n')
            c -= 13;
        // 如果是小写字母'a'到'm',则加上13
        else if (c >= 'a')
            c += 13;
        // 如果是大写字母'N'到'Z',则减去13
        else if (c >= 'N')
            c -= 13;
        // 如果是大写字母'A'到'M',则加上13
        else
            c += 13;
    }
    // 返回加密后的字符
    return (c);
}

int main(int argc, char *argv[])
{
    int ifd, ofd, i, n, nw;

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: rot13 infile outfile");
    // 打开输入文件
    if ((ifd = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't open %s", argv[1]);
    // 创建或打开输出文件
    if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("can't create %s", argv[2]);

    // 循环读取输入文件的数据
    while ((n = read(ifd, buf, BSZ)) > 0)
    {
        // 对读取到的每个字符进行加密
        for (i = 0; i < n; i++)
            buf[i] = translate(buf[i]);
        // 将加密后的数据写入输出文件
        if ((nw = write(ofd, buf, n)) != n)
        {
            // 如果写入失败,根据情况输出错误信息
            if (nw < 0)
                err_sys("write failed");
            else
                err_quit("short write (%d/%d)", nw, n);
        }
    }

    // 同步输出文件的数据到磁盘
    fsync(ofd);
    // 正常退出程序
    exit(0);
}
