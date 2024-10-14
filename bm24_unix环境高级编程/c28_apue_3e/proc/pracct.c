#include "apue.h"
#include <sys/acct.h>

// 根据不同的操作系统定义不同的结构体和格式化字符串
#if defined(BSD)                                                        /* 在FreeBSD中定义不同的结构体 */
#define acct    acctv2                                                  /* 定义acct为acctv2 */
#define ac_flag ac_trailer.ac_flag                                      /* 定义ac_flag为ac_trailer结构体中的ac_flag成员 */
#define FMT     "%-*.*s  e = %.0f, chars = %.0f, %c %c %c %c\n"         /* 定义格式化字符串,用于输出 */
#elif defined(HAS_AC_STAT)                                              /* 如果定义了HAS_AC_STAT */
#define FMT "%-*.*s  e = %6ld, chars = %7ld, stat = %3u: %c %c %c %c\n" /* 定义另一种格式化字符串,包含stat信息 */
#else                                                                   /* 其他情况 */
#define FMT "%-*.*s  e = %6ld, chars = %7ld, %c %c %c %c\n"             /* 定义默认的格式化字符串 */
#endif

#if defined(LINUX)   /* 如果是在Linux系统 */
#define acct acct_v3 /* 定义acct为acct_v3,适用于Linux的不同结构体 */
#endif

// 如果没有定义ACORE和AXSIG,则定义为0
#if !defined(HAS_ACORE)
#define ACORE 0
#endif
#if !defined(HAS_AXSIG)
#define AXSIG 0
#endif

// 如果不是BSD系统,定义compt2ulong函数,用于将comp_t类型转换为unsigned long
#if !defined(BSD)
/**
 * @brief 将comp_t类型的时间转换为unsigned long类型
 *
 * @param comptime comp_t类型的时间值
 * @return unsigned long 转换后的时间值
 */
static unsigned long
compt2ulong(comp_t comptime) /* convert comp_t to unsigned long */
{
    unsigned long val;  // 用于存储转换后的值
    int           exp;  // 指数部分

    val = comptime & 0x1fff;    /* 13位小数部分 */
    exp = (comptime >> 13) & 7; /* 3位指数部分 (0-7) */
    while (exp-- > 0)           // 根据指数部分调整值的大小
        val *= 8;
    return (val);  // 返回转换后的值
}
#endif

// 主函数
int main(int argc, char *argv[])
{
    struct acct acdata;  // 定义acct结构体变量
    FILE       *fp;      // 定义文件指针

    // 检查命令行参数数量,如果不是2个则打印使用方法并退出
    if (argc != 2)
        err_quit("usage: pracct filename");
    // 打开文件,如果失败则打印错误信息并退出
    if ((fp = fopen(argv[1], "r")) == NULL)
        err_sys("can't open %s", argv[1]);
    // 循环读取文件内容
    // 读取文件中的数据并打印相关信息
    while (fread(&acdata, sizeof(acdata), 1, fp) == 1)
    {
        // 根据不同的操作系统和定义的宏打印不同的信息
        // 打印ac_comm的大小和内容
        printf(FMT, (int)sizeof(acdata.ac_comm), (int)sizeof(acdata.ac_comm), acdata.ac_comm,
// 如果定义了BSD宏,打印ac_etime和ac_io的值
#if defined(BSD)
               acdata.ac_etime,
               acdata.ac_io,
// 否则,将ac_etime和ac_io的值转换为无符号长整型后再打印
#else
               compt2ulong(acdata.ac_etime),
               compt2ulong(acdata.ac_io),
#endif
// 如果定义了HAS_AC_STAT宏,打印ac_stat的值
#if defined(HAS_AC_STAT)
               (unsigned char)acdata.ac_stat,
#endif
               // 根据ac_flag的不同位打印相应的字符
               acdata.ac_flag & ACORE ? 'D' : ' ',
               acdata.ac_flag & AXSIG ? 'X' : ' ',
               acdata.ac_flag & AFORK ? 'F' : ' ',
               acdata.ac_flag & ASU ? 'S' : ' ');
    }

    // 如果读取文件时发生错误,则打印错误信息
    if (ferror(fp))
        err_sys("read error");
    // 正常退出程序
    exit(0);
}
