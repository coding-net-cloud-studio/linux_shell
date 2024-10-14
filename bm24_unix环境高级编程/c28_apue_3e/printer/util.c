// util.c

// 引入apue库的头文件,该库提供了一套用于编写可移植UNIX应用程序的函数.
#include "apue.h"

// 引入自定义的print库的头文件,可能包含自定义的打印函数.
#include "print.h"

// 引入ctype.h头文件,提供了字符处理函数,如isalpha,isdigit等.
#include <ctype.h>

// 引入sys/select.h头文件,包含了select函数的声明,用于I/O多路复用.
#include <sys/select.h>

// 定义了配置文件中每行最大长度为512个字符.
#define MAXCFGLINE 512

// 定义了关键字的最大长度为16个字符.
#define MAXKWLEN   16

// 定义了格式化字符串的最大长度为16个字符.
#define MAXFMTLEN  16

/*
 * 获取给定主机和服务的地址列表,并通过ailistpp返回.
 * 成功返回0,失败返回错误代码.注意,如果遇到错误,我们不会设置errno.
 *
 * 锁定:无.
 */
int getaddrlist(const char *host, const char *service, struct addrinfo **ailistpp)
{
    int             err;   // 用于存储getaddrinfo函数的返回值
    struct addrinfo hint;  // 初始化一个addrinfo结构体,用于传递给getaddrinfo函数

    // 设置hint结构体的成员变量
    hint.ai_flags     = AI_CANONNAME;  // 请求规范主机名
    hint.ai_family    = AF_INET;       // 使用IPv4地址族
    hint.ai_socktype  = SOCK_STREAM;   // 使用流式套接字
    hint.ai_protocol  = 0;             // 协议由系统选择
    hint.ai_addrlen   = 0;             // 地址长度,将在getaddrinfo中设置
    hint.ai_canonname = NULL;          // 规范主机名,将在getaddrinfo中设置
    hint.ai_addr      = NULL;          // 地址,将在getaddrinfo中设置
    hint.ai_next      = NULL;          // 指向下一个addrinfo结构的指针

    // 调用getaddrinfo函数,获取地址列表
    err = getaddrinfo(host, service, &hint, ailistpp);
    return (err);  // 返回getaddrinfo的结果
}

/*
 * 给定一个关键字,扫描配置文件以找到匹配项,并返回与该关键字对应的字符串值.
 *
 * 锁定:无.
 */
static char *
scan_configfile(char *keyword)
{
    int         n, match;                              // n用于sscanf的返回值,match用于标记是否找到匹配项
    FILE       *fp;                                    // 文件指针
    char        keybuf[MAXKWLEN], pattern[MAXFMTLEN];  // keybuf用于存储读取的关键字,pattern用于构建sscanf的格式字符串
    char        line[MAXCFGLINE];                      // line用于存储从文件中读取的一行数据
    static char valbuf[MAXCFGLINE];                    // valbuf用于存储与关键字对应的值

    if ((fp = fopen(CONFIG_FILE, "r")) == NULL)                     // 打开配置文件
        log_sys("can't open %s", CONFIG_FILE);                      // 如果文件打开失败,记录错误日志
    sprintf(pattern, "%%%ds %%%ds", MAXKWLEN - 1, MAXCFGLINE - 1);  // 构建用于sscanf的格式字符串
    match = 0;                                                      // 初始化匹配标记为0
    while (fgets(line, MAXCFGLINE, fp) != NULL)                     // 逐行读取文件内容
    {
        n = sscanf(line, pattern, keybuf, valbuf);   // 尝试从当前行中解析出关键字和值
        if (n == 2 && strcmp(keyword, keybuf) == 0)  // 如果解析出两个数据项且关键字匹配
        {
            match = 1;  // 设置匹配标记为1
            break;      // 跳出循环
        }
    }
    fclose(fp);           // 关闭文件
    if (match != 0)       // 如果找到匹配项
        return (valbuf);  // 返回对应的值
    else
        return (NULL);  // 否则返回NULL
}

/*
 * 返回运行打印服务器的主机名,如果出错则返回NULL.
 *
 * 锁定:无.
 */
// 函数功能:获取运行打印服务器的主机名
// 参数:无
// 返回值:成功返回主机名字符串指针,失败返回NULL
char *
get_printserver(void)
{
    // 调用scan_configfile函数,尝试从配置文件中读取打印服务器的主机名
    return (scan_configfile("printserver"));
}

/*
 * 返回网络打印机的地址,如果出错则返回NULL.
 *
 * 锁定:无.
 */

// 定义函数get_printaddr,用于获取网络打印机的地址信息
struct addrinfo *
get_printaddr(void)
{
    int              err;     // 用于存储错误码
    char            *p;       // 用于存储配置文件中打印机地址的指针
    struct addrinfo *ailist;  // 用于存储解析后的地址信息列表

    // 尝试从配置文件中扫描打印机地址
    if ((p = scan_configfile("printer")) != NULL)
    {
        // 如果成功获取到地址,则尝试解析该地址
        if ((err = getaddrlist(p, "ipp", &ailist)) != 0)
        {
            // 如果解析失败,记录错误信息并返回NULL
            log_msg("no address information for %s", p);
            return (NULL);
        }
        // 解析成功,返回地址信息列表
        return (ailist);
    }
    // 如果配置文件中没有指定打印机地址,记录错误信息并返回NULL
    log_msg("no printer address specified");
    return (NULL);
}
/*
 * "定时"读取 - timout 指定在放弃之前等待的秒数(select 的第五个参数控制等待数据可读的时间).
 * 返回读取的字节数或在错误时返回 -1.
 *
 * 锁定:无.
 */
ssize_t
tread(int fd, void *buf, size_t nbytes, unsigned int timout)
{
    int            nfds;     // 文件描述符集合中的文件描述符数量
    fd_set         readfds;  // 文件描述符集合,用于select函数
    struct timeval tv;       // 结构体,用于设置select函数的超时时间

    tv.tv_sec  = timout;                               // 设置超时的秒数
    tv.tv_usec = 0;                                    // 设置超时的微秒数,这里设置为0
    FD_ZERO(&readfds);                                 // 清空文件描述符集合
    FD_SET(fd, &readfds);                              // 将要监视的文件描述符添加到集合中
    nfds = select(fd + 1, &readfds, NULL, NULL, &tv);  // 调用select函数,监视文件描述符集合中的文件描述符
    if (nfds <= 0)                                     // 如果返回值小于等于0,表示出错或超时
    {
        if (nfds == 0)      // 如果返回值为0,表示超时
            errno = ETIME;  // 设置errno为ETIME,表示操作超时
        return (-1);        // 返回-1,表示读取失败
    }
    return (read(fd, buf, nbytes));  // 调用read函数读取数据,并返回读取的字节数
}

/*
 * "定时"读取 - timout 指定每次 read 调用前等待的秒数,在放弃前必须读取 nbytes 字节.
 * 返回读取的字节数或在错误时返回 -1.
 *
 * 锁定:无.
 */
ssize_t
treadn(int fd, void *buf, size_t nbytes, unsigned int timout)
{
    size_t  nleft;  // 剩余需要读取的字节数
    ssize_t nread;  // 每次 read 调用实际读取的字节数

    nleft = nbytes;    // 初始化剩余字节数为总字节数
    while (nleft > 0)  // 当还有字节需要读取时循环
    {
        if ((nread = tread(fd, buf, nleft, timout)) < 0)  // 尝试读取数据
        {
            if (nleft == nbytes)  // 如果是第一次尝试就读取失败,则返回 -1 表示错误
                return (-1);
            else  // 否则,表示发生了可恢复的错误,跳出循环
                break;
        }
        else if (nread == 0)  // 如果读取到文件末尾,也跳出循环
        {
            break;
        }
        nleft -= nread;  // 更新剩余需要读取的字节数
        buf += nread;    // 更新缓冲区指针位置
    }
    return (nbytes - nleft);  // 返回实际读取的字节数
}
