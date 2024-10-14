/*
 * 我们自己的头文件,在包含所有标准系统头文件之前被包含.
 */
#ifndef _APUE_H  // 如果没有定义_APUE_H,则定义它,防止头文件被重复包含
#define _APUE_H

#define _POSIX_C_SOURCE 200809L  // 定义_POSIX_C_SOURCE为200809L,指明使用POSIX.1-2008标准

#if defined(SOLARIS) /* Solaris 10 */  // 如果定义了SOLARIS,表示代码运行在Solaris 10系统上
#define _XOPEN_SOURCE 600              // 则定义_XOPEN_SOURCE为600
#else
#define _XOPEN_SOURCE 700  // 否则定义_XOPEN_SOURCE为700
#endif

// 以上代码用于确保在不同的操作系统平台上能够正确地包含所需的系统特性和库

#include <sys/types.h>   /* 一些系统仍然需要这个 */
#include <sys/stat.h>    /* 文件状态信息 */
#include <sys/termios.h> /* 用于获取窗口大小信息 */

#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h> /* 在某些系统上,可能需要包含此头文件来获取窗口大小 */
#endif

#include <stdio.h>  /* 方便起见,包含标准输入输出 */
#include <stdlib.h> /* 方便起见,包含标准库函数 */
#include <stddef.h> /* 包含offsetof宏,用于获取结构体成员的偏移量 */
#include <string.h> /* 方便起见,包含字符串处理函数 */
#include <unistd.h> /* 方便起见,包含unistd函数 */
#include <signal.h> /* 包含信号处理函数,如SIG_ERR */

#define MAXLINE   4096 /* 定义最大行长度为4096 */

/*
 * 默认文件访问权限,用于新创建的文件.
 * S_IRUSR: 文件所有者具有读权限
 * S_IWUSR: 文件所有者具有写权限
 * S_IRGRP: 文件所属组具有读权限
 * S_IROTH: 其他用户具有读权限
 */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * 默认权限,用于新创建的目录.
 * 在文件权限的基础上增加了执行权限:
 * S_IXUSR: 文件所有者具有执行权限
 * S_IXGRP: 文件所属组具有执行权限
 * S_IXOTH: 其他用户具有执行权限
 */
#define DIR_MODE  (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

// 定义信号处理函数的类型,接受一个整型参数
typedef void Sigfunc(int); /* for signal handlers */

/*
 * 返回两个整数中的最小值
 * @param a 第一个整数
 * @param b 第二个整数
 * @return a和b中的最小值
 */
#define min(a, b) ((a) < (b) ? (a) : (b))

/*
 * 返回两个整数中的最大值
 * @param a 第一个整数
 * @param b 第二个整数
 * @return a和b中的最大值
 */
#define max(a, b) ((a) > (b) ? (a) : (b))

/*
 * Prototypes for our own functions.
 */

// 分配路径内存
// 参数: size_t * - 指向所需内存大小的指针
// 返回: char * - 分配的内存路径字符串指针
char *path_alloc(size_t *); /* {Prog pathalloc} */

// 获取系统打开文件的最大数量
// 返回: long - 系统打开文件的最大数量
long open_max(void); /* {Prog openmax} */

// 设置文件描述符的执行时关闭标志
// 参数: int - 文件描述符
// 返回: int - 成功返回0,失败返回-1
int set_cloexec(int); /* {Prog setfd} */

// 清除文件描述符的指定标志
// 参数: int - 文件描述符, int - 要清除的标志
void clr_fl(int, int);

// 设置文件描述符的指定标志
// 参数: int - 文件描述符, int - 要设置的标志
void set_fl(int, int); /* {Prog setfl} */

// 打印退出状态
// 参数: int - 退出状态码
void pr_exit(int); /* {Prog prexit} */

// 打印当前信号屏蔽字
// 参数: const char * - 标题字符串
void pr_mask(const char *); /* {Prog prmask} */

// 设置信号处理函数,并返回之前的信号处理函数
// 参数: int - 信号编号, Sigfunc * - 新的信号处理函数指针
// 返回: Sigfunc * - 之前的信号处理函数指针
Sigfunc *signal_intr(int, Sigfunc *); /* {Prog signal_intr_function} */

// 将进程转换为守护进程
// 参数: const char * - 守护进程的工作目录路径
void daemonize(const char *); /* {Prog daemoninit} */

// 微秒级睡眠函数
// 参数: unsigned int - 睡眠的微秒数
void sleep_us(unsigned int); /* {Ex sleepus} */

// 读取指定数量的字节
// 参数: int - 文件描述符, void * - 存储读取数据的缓冲区指针, size_t - 要读取的字节数
// 返回: ssize_t - 实际读取的字节数
ssize_t readn(int, void *, size_t); /* {Prog readn_writen} */

// 写入指定数量的字节
// 参数: int - 文件描述符, const void * - 要写入数据的缓冲区指针, size_t - 要写入的字节数
// 返回: ssize_t - 实际写入的字节数
ssize_t writen(int, const void *, size_t); /* {Prog readn_writen} */

// figB.1.c

// 创建一个管道并返回文件描述符
// 参数: 指向整数的指针,用于存储创建的管道的文件描述符
// 返回值: 成功返回0,失败返回-1
int fd_pipe(int *);

// 从套接字接收文件描述符
// 参数: 套接字描述符,指向函数的指针,该函数用于处理接收到的数据
// 返回值: 成功返回接收到的文件描述符,失败返回-1
int recv_fd(int, ssize_t (*func)(int, const void *, size_t));

// 通过套接字发送文件描述符
// 参数: 套接字描述符,要发送的文件描述符
// 返回值: 成功返回0,失败返回-1
int send_fd(int, int);

// 发送错误信息
// 参数: 套接字描述符,错误码,错误信息字符串
// 返回值: 成功返回0,失败返回-1
int send_err(int, int, const char *);

// 服务器监听端口
// 参数: 端口字符串
// 返回值: 成功返回监听套接字描述符,失败返回-1
int serv_listen(const char *);

// 服务器接受连接
// 参数: 监听套接字描述符,指向用户ID的指针
// 返回值: 成功返回新的连接套接字描述符,失败返回-1
int serv_accept(int, uid_t *);

// 客户端连接服务器
// 参数: 服务器地址字符串
// 返回值: 成功返回连接套接字描述符,失败返回-1
int cli_conn(const char *);

// 处理缓冲区参数
// 参数: 字符数组,指向函数的指针,该函数用于处理命令行参数
// 返回值: 成功返回0,失败返回-1
int buf_args(char *, int (*func)(int, char **));

// 将终端设置为cbreak模式
// 参数: 终端文件描述符
// 返回值: 成功返回0,失败返回-1
int tty_cbreak(int);

// 将终端设置为原始模式
// 参数: 终端文件描述符
// 返回值: 成功返回0,失败返回-1
int tty_raw(int);

// 重置终端设置
// 参数: 终端文件描述符
// 返回值: 成功返回0,失败返回-1
int tty_reset(int);

// 注册tty_atexit函数,用于在程序退出时恢复终端设置
void tty_atexit(void);

// 获取当前终端的termios结构体指针
// 返回值: 指向termios结构体的指针
struct termios *tty_termios(void);

// ptym_open 函数用于打开一个伪终端主设备
// 参数:char * - 设备名称; int - 打开标志
int ptym_open(char *, int); /* {Prog ptyopen} */

// ptys_open 函数用于打开一个伪终端从设备
// 参数:char * - 设备名称
int ptys_open(char *); /* {Prog ptyopen} */

// #ifdef TIOCGWINSZ 定义了 TIOCGWINSZ 宏,如果定义了,则下面的函数才会被编译
#ifdef TIOCGWINSZ
// pty_fork 函数用于创建一个子进程,并将其与伪终端关联
// 参数:int * - 子进程的PID; char * - 伪终端主设备的名称; int - 打开标志;
//       const struct termios * - 终端属性; const struct winsize * - 窗口大小
pid_t pty_fork(int *, char *, int, const struct termios *, const struct winsize *); /* {Prog ptyfork} */
#endif

// lock_reg 函数用于对文件区域加锁或解锁
// 参数:int - 文件描述符; int - 命令; int - 锁类型; off_t - 起始偏移量;
//       int - 锁定的起始位置; off_t - 锁定的长度
int lock_reg(int, int, int, off_t, int, off_t); /* {Prog lockreg} */

// read_lock 宏定义用于对文件区域加读锁
#define read_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))

// readw_lock 宏定义用于对文件区域加读锁,并阻塞直到锁成功
#define readw_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))

// write_lock 宏定义用于对文件区域加写锁
#define write_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))

// writew_lock 宏定义用于对文件区域加写锁,并阻塞直到锁成功
#define writew_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))

// un_lock 宏定义用于对文件区域解锁
#define un_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

// lock_test 函数用于测试文件区域是否已经被加锁
// 参数:int - 文件描述符; int - 锁类型; off_t - 起始偏移量; int - 锁定的起始位置;
//       off_t - 锁定的长度
pid_t lock_test(int, int, off_t, int, off_t); /* {Prog locktest} */

// 定义宏is_read_lockable,用于检查文件描述符fd在指定的偏移量,起始位置和长度上是否可读锁定
#define is_read_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)

// 定义宏is_write_lockable,用于检查文件描述符fd在指定的偏移量,起始位置和长度上是否可写锁定
#define is_write_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

// 错误消息处理函数,用于输出错误信息
void err_msg(const char *, ...); /* {App misc_source} */

// 错误转储处理函数,用于输出详细的错误信息并终止程序
void err_dump(const char *, ...) __attribute__((noreturn));

// 错误退出处理函数,用于输出错误信息并终止程序
void err_quit(const char *, ...) __attribute__((noreturn));

// 错误继续处理函数,用于在程序继续执行前输出错误信息
void err_cont(int, const char *, ...);

// 错误退出处理函数,带退出码,用于输出错误信息并终止程序
void err_exit(int, const char *, ...) __attribute__((noreturn));

// 错误返回处理函数,用于输出错误信息并返回
void err_ret(const char *, ...);

// 系统错误处理函数,用于输出系统错误信息并终止程序
void err_sys(const char *, ...) __attribute__((noreturn));

// 日志消息处理函数,用于记录日志信息
void log_msg(const char *, ...); /* {App misc_source} */

// 日志打开处理函数,用于打开日志文件
void log_open(const char *, int, int);

// 日志退出处理函数,用于记录日志信息并终止程序
void log_quit(const char *, ...) __attribute__((noreturn));

// 日志返回处理函数,用于记录日志信息并返回
void log_ret(const char *, ...);

// 系统日志处理函数,用于记录系统日志信息并终止程序
void log_sys(const char *, ...) __attribute__((noreturn));

// 日志退出处理函数,带退出码,用于记录日志信息并终止程序
void log_exit(int, const char *, ...) __attribute__((noreturn));

// figB.1.c

/**
 * @brief 父子进程同步函数,具体实现未给出.
 */
void TELL_WAIT(void); /* parent/child from {Sec race_conditions} */

/**
 * @brief 通知父进程某个事件已经发生.
 * @param pid_t pid 子进程的进程ID.
 */
void TELL_PARENT(pid_t);

/**
 * @brief 通知子进程某个事件已经发生.
 * @param pid_t pid 父进程的进程ID.
 */
void TELL_CHILD(pid_t);

/**
 * @brief 等待父进程的通知.
 */
void WAIT_PARENT(void);

/**
 * @brief 等待子进程的通知.
 */
void WAIT_CHILD(void);

#endif /* _APUE_H */
