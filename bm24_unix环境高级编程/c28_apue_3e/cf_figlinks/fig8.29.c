#include "apue.h"
#include <sys/acct.h>

// 根据不同的操作系统定义不同的结构体和格式化字符串
#if defined(BSD) /* 不同的结构体在FreeBSD中 */
// 定义acct为acctv2,ac_flag为ac_trailer结构体中的ac_flag
#define acct    acctv2
#define ac_flag ac_trailer.ac_flag
// 定义格式化字符串FMT,用于输出进程信息
#define FMT     "%-*.*s  e = %.0f, chars = %.0f, %c %c %c %c\n"
#elif defined(HAS_AC_STAT)
// 如果系统支持ac_stat,则定义不同的格式化字符串FMT
#define FMT "%-*.*s  e = %6ld, chars = %7ld, stat = %3u: %c %c %c %c\n"
#else
// 默认的格式化字符串FMT
#define FMT "%-*.*s  e = %6ld, chars = %7ld, %c %c %c %c\n"
#endif

// 如果系统不支持ACORE,则定义ACORE为0
#if !defined(HAS_ACORE)
#define ACORE 0
#endif
// 如果系统不支持AXSIG,则定义AXSIG为0
#if !defined(HAS_AXSIG)
#define AXSIG 0
#endif

#if !defined(BSD)
// 函数:compt2ulong
// 功能:将comp_t类型的值转换为unsigned long类型
// 参数:
//   comptime - comp_t类型的值,需要被转换
// 返回值:
//   转换后的unsigned long类型的值
static unsigned long
compt2ulong(comp_t comptime) /* convert comp_t to unsigned long */
{
    unsigned long val;  // 用于存储转换后的值
    int           exp;  // 用于存储指数部分

    val = comptime & 0x1fff;    /* 13位分数部分 */
    exp = (comptime >> 13) & 7; /* 3位指数部分 (0-7) */
    while (exp-- > 0)           // 根据指数部分调整val的值
        val *= 8;
    return (val);  // 返回转换后的值
}
#endif
// fig8.29.c

/**
 * 主函数,读取账户信息文件并打印账户详细信息.
 * @param argc 命令行参数的数量
 * @param argv 命令行参数的数组
 * @return 程序退出状态码
 */
int main(int argc, char *argv[])
{
    struct acct acdata;  // 定义账户数据结构体变量
    FILE       *fp;      // 文件指针

    // 检查命令行参数数量是否正确
    if (argc != 2)
        err_quit("usage: pracct filename");

    // 尝试打开文件
    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);

    // 循环读取文件中的账户数据
    while (fread(&acdata, sizeof(acdata), 1, fp) == 1)
    {
        // 打印账户信息,根据不同的系统定义打印不同的字段
        // 打印进程控制块信息
        // FMT 是一个格式化字符串,用于定义输出的格式
        // acdata.ac_comm 是进程的命令名
        // sizeof(acdata.ac_comm) 获取命令名的大小
        // acdata.ac_etime 是进程的执行时间
        // acdata.ac_io 是进程的 I/O 数量
        // acdata.ac_stat 是进程的状态
        // acdata.ac_flag 是进程的标志位
        printf(FMT, (int)sizeof(acdata.ac_comm), (int)sizeof(acdata.ac_comm), acdata.ac_comm,
#if defined(BSD)
               // 如果定义了 BSD,直接打印执行时间和 I/O 数量
               acdata.ac_etime,
               acdata.ac_io,
#else
               // 否则,将执行时间和 I/O 数量转换为无符号长整型再打印
               compt2ulong(acdata.ac_etime),
               compt2ulong(acdata.ac_io),
#endif
#if defined(HAS_AC_STAT)
               // 如果定义了 HAS_AC_STAT,打印进程状态
               (unsigned char)acdata.ac_stat,
#endif
               // 根据进程标志位打印相应的字符
               acdata.ac_flag & ACORE ? 'D' : ' ',
               acdata.ac_flag & AXSIG ? 'X' : ' ',
               acdata.ac_flag & AFORK ? 'F' : ' ',
               acdata.ac_flag & ASU ? 'S' : ' ');
    }

    // 检查文件读取过程中是否发生错误
    if (ferror(fp))
        err_sys("read error");

    // 正常退出程序
    exit(0);
}
