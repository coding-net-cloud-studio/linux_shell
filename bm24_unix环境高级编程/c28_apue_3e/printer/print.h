#ifndef _PRINT_H
#define _PRINT_H

/*
 * Print server header file.
 */
// 包含套接字编程所需的头文件
#include <sys/socket.h>  // 提供套接字创建和操作的函数
#include <arpa/inet.h>   // 提供IP地址转换函数
#include <netdb.h>       // 提供主机名解析函数
#include <errno.h>       // 提供错误码定义和错误处理函数

// 定义配置文件的路径
#define CONFIG_FILE "/etc/printer.conf"

// 定义打印队列的目录
#define SPOOLDIR    "/var/spool/printer"

// 定义存储作业编号的文件名
#define JOBFILE     "jobno"

// 定义数据文件的目录
#define DATADIR     "data"

// 定义请求文件的目录
#define REQDIR      "reqs"

// 根据不同的操作系统定义打印机服务的名称
#if defined(BSD)         // 如果是BSD系统
#define LPNAME "daemon"  // 定义打印机服务名称为"daemon"
#elif defined(MACOS)     // 如果是macOS系统
#define LPNAME "_lp"     // 定义打印机服务名称为"_lp"
#else                    // 其他系统
#define LPNAME "lp"      // 定义打印机服务名称为"lp"
#endif

// 定义文件名最大长度
#define FILENMSZ   64
// 定义文件权限,仅用户可读写
#define FILEPERM   (S_IRUSR | S_IWUSR)

// 定义用户名最大长度
#define USERNM_MAX 64
// 定义作业名最大长度
#define JOBNM_MAX  256
// 定义消息长度最大值
#define MSGLEN_MAX 512

// 如果未定义主机名最大长度,则定义之
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

// 定义IPP服务端口
#define IPP_PORT 631
// 定义队列长度
#define QLEN     10

// 定义IPP头部缓冲区大小
#define IBUFSZ   512
// 定义HTTP头部缓冲区大小
#define HBUFSZ   512
// 定义数据缓冲区大小
#define IOBUFSZ  8192

// 以下代码定义了一个宏 ETIME,如果未定义 ETIME,则将其定义为 ETIMEDOUT.
// 这通常用于处理超时相关的错误码.

#ifndef ETIME            // 如果 ETIME 没有被定义
#define ETIME ETIMEDOUT  // 则定义 ETIME 为 ETIMEDOUT
#endif                   // 结束条件编译指令

// print.h

// 获取地址列表,参数分别为主机名,服务名和指向addrinfo结构体指针的指针
// 返回值为int类型,表示操作成功与否
extern int getaddrlist(const char *, const char *, struct addrinfo **);

// 获取打印服务器地址,无参数
// 返回值为指向char的指针,表示打印服务器的地址
extern char *get_printserver(void);

// 获取打印地址信息,无参数
// 返回值为指向addrinfo结构体的指针,表示打印地址的信息
extern struct addrinfo *get_printaddr(void);

// 从文件描述符读取数据,参数包括文件描述符,缓冲区,读取字节数和标志位
// 返回值为ssize_t类型,表示实际读取的字节数
extern ssize_t tread(int, void *, size_t, unsigned int);

// 带超时的读取函数,参数与tread相同,增加了超时时间
// 返回值同样为ssize_t类型,表示实际读取的字节数
extern ssize_t treadn(int, void *, size_t, unsigned int);

// 尝试连接服务器,参数包括文件描述符,地址族,套接字类型,地址结构体和地址长度
// 返回值为int类型,表示连接成功与否
extern int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

// 初始化服务器,参数包括监听端口,地址结构体,地址长度和最大连接数
// 返回值为int类型,表示初始化成功与否
extern int initserver(int, const struct sockaddr *, socklen_t, int);

/*
 * 结构体描述打印请求.
 */
struct printreq
{
    uint32_t size;                // 请求的文件大小,以字节为单位
    uint32_t flags;               // 打印请求的标志位
    char     usernm[USERNM_MAX];  // 用户名
    char     jobnm[JOBNM_MAX];    // 打印任务的名称
};
/*
 * 请求标志.
 */
#define PR_TEXT 0x01 /* 将文件视为纯文本 */

/*
 * 打印命令的假脱机守护程序响应.
 */
struct printresp
{
    uint32_t retcode;         /* 0=成功, !=0=错误代码 */
    uint32_t jobid;           /* 作业ID */
    char     msg[MSGLEN_MAX]; /* 错误消息 */
};

#endif /* _PRINT_H */
