#include "apue.h"      // 引入APUE库头文件
#include <fcntl.h>     // 引入文件控制操作的头文件
#include <sys/mman.h>  // 引入内存映射操作的头文件

#define COPYINCR (1024 * 1024 * 1024) /* 1 GB */  // 定义COPYINCR为1GB,用于内存映射时的增量大小

int main(int argc, char *argv[])
{
    int         fdin, fdout;  // 输入和输出文件的文件描述符
    void       *src, *dst;    // 源地址和目标地址的内存映射指针
    size_t      copysz;       // 每次复制的字节数
    struct stat sbuf;         // 文件状态结构体,用于存储文件信息
    off_t       fsz = 0;      // 已复制的文件大小

    // 检查命令行参数数量是否正确
    if (argc != 3)
        err_quit("usage: %s <fromfile> <tofile>", argv[0]);

    // 打开输入文件进行读取
    if ((fdin = open(argv[1], O_RDONLY)) < 0)
        err_sys("can't open %s for reading", argv[1]);

    // 创建或打开输出文件进行读写,并截断到0长度
    // 打开文件进行读写,如果文件不存在则创建,如果存在则截断为0长度
    // 参数argv[2]是要打开或创建的文件名
    // FILE_MODE是文件的权限模式
    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0)
        // 如果打开或创建文件失败,调用err_sys函数输出错误信息
        err_sys("can't creat %s for writing", argv[2]);

    // 获取输入文件的状态,主要是为了获取文件大小
    if (fstat(fdin, &sbuf) < 0)
        err_sys("fstat error");

    // 设置输出文件的大小与输入文件相同
    // 使用 ftruncate 函数调整文件描述符 fdout 指向的文件大小为 sbuf.st_size
    // 如果操作失败,调用 err_sys 函数输出错误信息 "ftruncate error"
    if (ftruncate(fdout, sbuf.st_size) < 0)
        err_sys("ftruncate error");

    // 循环复制文件内容,直到整个文件被复制完毕
    while (fsz < sbuf.st_size)
    {
        // 根据剩余未复制的文件大小决定本次复制的字节数
        // 如果剩余文件大小大于COPYINCR,则复制COPYINCR大小的数据
        if ((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        // 否则,复制剩余的所有文件数据
        else
            copysz = sbuf.st_size - fsz;

        // 将输入文件的一部分映射到内存中
        // 使用mmap函数将文件映射到内存中
        // 参数解释:
        // 0: 映射区域的起始地址,0表示由系统自动选择
        // copysz: 映射区域的大小,即要复制的字节数
        // PROT_READ: 映射区域的权限,此处为只读
        // MAP_SHARED: 映射类型,表示映射区域是共享的
        // fdin: 文件描述符,表示要映射的文件
        // fsz: 文件大小
        if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED)
            // 如果mmap失败,调用err_sys函数输出错误信息
            err_sys("mmap error for input");

        // 将输出文件的一部分映射到内存中,以便写入
        // 如果映射失败,err_sys函数将被调用以报告错误
        // dst:映射后的内存地址
        // copysz:要映射的字节数
        // PROT_READ | PROT_WRITE:映射区域可读可写
        // MAP_SHARED:映射区域为共享内存
        // fdout:文件描述符,指向要映射的文件
        // fsz:文件大小
        if ((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED)
            err_sys("mmap error for output");  // 映射输出时的mmap错误

        // 执行内存复制操作
        memcpy(dst, src, copysz);

        // 解除内存映射,释放资源
        // 释放源地址空间
        munmap(src, copysz);  // 该函数用于解除之前通过mmap映射到进程地址空间的文件或设备的映射关系
        // 释放目标地址空间
        munmap(dst, copysz);  // 同上,解除目标地址空间的映射关系

        // 更新已复制的文件大小
        fsz += copysz;
    }
    // 正常退出程序
    exit(0);
}
