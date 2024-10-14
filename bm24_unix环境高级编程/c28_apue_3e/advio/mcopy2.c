#include "apue.h"      // 引入APUE库头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include <fcntl.h>     // 引入文件控制相关的头文件,包括open,creat等函数.
#include <sys/mman.h>  // 引入内存映射相关的头文件,包括mmap,munmap等函数.

#define COPYINCR (1024 * 1024 * 1024) /* 1 GB */

/**
 * 主函数,用于将一个文件的内容复制到另一个文件.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    int         fdin, fdout;  // 输入和输出文件的文件描述符
    void       *src, *dst;    // 源文件和目标文件的内存映射地址
    size_t      copysz;       // 每次复制的字节数
    struct stat sbuf;         // 文件状态结构体,用于存储文件信息
    off_t       fsz = 0;      // 已复制的字节数

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: %s <fromfile> <tofile>", argv[0]);

    // 打开源文件用于读取
    if ((fdin = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't open %s for reading", argv[1]);

    // 创建目标文件用于写入,如果文件已存在则截断
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        err_sys("can't creat %s for writing", argv[2]);

    // 获取源文件的状态信息,主要是文件大小
    if (fstat(fdin, &sbuf) < 0)
        err_sys("fstat error");

    // 设置目标文件的大小与源文件相同
    // 使用 ftruncate 函数调整文件描述符 fdout 指向的文件大小为 sbuf.st_size
    // 如果操作失败,调用 err_sys 函数报告错误 "ftruncate error"
    if (ftruncate(fdout, sbuf.st_size) < 0)
        err_sys("ftruncate error");

    // 循环复制文件内容,直到全部复制完毕
    while (fsz < sbuf.st_size)
    {
        // 计算本次复制的字节数
        // 如果剩余的文件大小大于COPYINCR,则设置copysz为COPYINCR
        // 否则,设置copysz为剩余的文件大小
        if ((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        else
            copysz = sbuf.st_size - fsz;

        // 将源文件映射到内存中
        if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED)
            err_sys("mmap error for input");

        // 将目标文件映射到内存中
        if ((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED)
            err_sys("mmap error for output");

        // 复制内存中的数据
        memcpy(dst, src, copysz);

        // 解除内存映射
        // 释放源地址空间
        // 参数src: 源地址
        // 参数copysz: 要释放的内存大小
        munmap(src, copysz);

        // 释放目标地址空间
        // 参数dst: 目标地址
        // 参数copysz: 要释放的内存大小
        munmap(dst, copysz);

        // 更新已复制的字节数
        fsz += copysz;
    }
    // 正常退出程序
    exit(0);
}
