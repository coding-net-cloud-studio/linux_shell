/*
 * 我们自己的头文件,在包含所有标准系统头文件之前包含.
 */
#ifndef _APUE_H  // 如果没有定义_APUE_H,则定义它,防止头文件被重复包含
#define _APUE_H

#define _POSIX_C_SOURCE 200809L  // 定义_POSIX_C_SOURCE为200809L,指定符合POSIX.1-2008标准

#if defined(SOLARIS) /* Solaris 10 */  // 如果定义了SOLARIS,表示在Solaris 10系统上
#define _XOPEN_SOURCE 600              // 定义_XOPEN_SOURCE为600
#else
#define _XOPEN_SOURCE 700  // 否则定义_XOPEN_SOURCE为700
#endif

#include <sys/types.h>   /* 一些系统仍然需要这个头文件 */
#include <sys/stat.h>    /* 提供文件状态信息 */
#include <sys/termios.h> /* 提供终端I/O控制功能,例如winsize */

/* 如果定义了MACOS或者没有定义TIOCGWINSZ,则包含ioctl.h头文件,
   ioctl.h提供了对设备特定功能的访问,如获取窗口大小等 */
#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

// 为了方便,包含标准输入输出库
#include <stdio.h>

// 为了方便,包含标准库,提供各种实用函数
#include <stdlib.h>

// 包含offsetof宏,用于获取结构体成员的偏移量
#include <stddef.h>

// 为了方便,包含字符串处理库
#include <string.h>

// 为了方便,包含Unix系统调用库
#include <unistd.h>

// 包含信号处理库,其中SIG_ERR是信号处理函数的错误返回值
#include <signal.h>

/* # wmtag_memo_我修改了这里_开始 */
#include "error.h"
/* # wmtag_memo_我修改了这里_结束 */
// 定义最大行长度为4096字节
#define MAXLINE   4096 /* max line length */

/*
 * 新文件的默认文件访问权限.
 * S_IRUSR: 文件所有者具有读取权限
 * S_IWUSR: 文件所有者具有写入权限
 * S_IRGRP: 文件所属组具有读取权限
 * S_IROTH: 其他用户具有读取权限
 */
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * 新目录的默认权限.
 * 在FILE_MODE的基础上增加了执行权限:
 * S_IXUSR: 文件所有者具有执行权限
 * S_IXGRP: 文件所属组具有执行权限
 * S_IXOTH: 其他用户具有执行权限
 */
#define DIR_MODE  (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

// 定义信号处理函数的类型,接受一个整型参数
typedef void Sigfunc(int); /* for signal handlers */

// 定义宏min,返回两个参数中较小的一个
#define min(a, b) ((a) < (b) ? (a) : (b))

// 定义宏max,返回两个参数中较大的一个
#define max(a, b) ((a) > (b) ? (a) : (b))
/*
 * Prototypes for our own functions.
 */

// 分配路径内存
// 参数: size_t * - 用于存储分配的内存大小的指针
// 返回: 分配的路径字符串指针
char *path_alloc(size_t *); /* {Prog pathalloc} */

// 获取系统允许的最大文件描述符数
// 返回: 最大文件描述符数
long open_max(void); /* {Prog openmax} */

// 设置文件描述符的执行时关闭标志
// 参数: int fd - 文件描述符
// 返回: 成功返回0,失败返回-1
int set_cloexec(int); /* {Prog setfd} */

// 清除文件描述符的指定标志
// 参数: int fd - 文件描述符, int flags - 要清除的标志
void clr_fl(int, int);

// 设置文件描述符的指定标志
// 参数: int fd - 文件描述符, int flags - 要设置的标志
void set_fl(int, int); /* {Prog setfl} */

// 打印退出状态
// 参数: int status - 退出状态
void pr_exit(int); /* {Prog prexit} */

// 打印当前信号屏蔽字
// 参数: const char *msg - 打印的消息
void pr_mask(const char *); /* {Prog prmask} */

// 设置信号处理函数,并在接收到中断信号时返回
// 参数: int signo - 信号编号, Sigfunc *func - 信号处理函数指针
// 返回: 原信号处理函数指针
Sigfunc *signal_intr(int, Sigfunc *); /* {Prog signal_intr_function} */

// 将进程转换为守护进程
// 参数: const char *pname - 进程名称
void daemonize(const char *); /* {Prog daemoninit} */

// 微秒级睡眠函数
// 参数: unsigned int nusec - 睡眠的微秒数
void sleep_us(unsigned int); /* {Ex sleepus} */

// 读取指定数量的字节
// 参数: int fd - 文件描述符, void *ptr - 存储读取数据的指针, size_t nbytes - 要读取的字节数
// 返回: 实际读取的字节数
ssize_t readn(int, void *, size_t); /* {Prog readn_writen} */

// 写入指定数量的字节
// 参数: int fd - 文件描述符, const void *ptr - 要写入数据的指针, size_t nbytes - 要写入的字节数
// 返回: 实际写入的字节数
ssize_t writen(int, const void *, size_t); /* {Prog readn_writen} */

// 创建一个管道,并返回两个文件描述符
// 参数: int *ptr - 存储两个管道文件描述符的数组指针
// 返回: 成功返回0,失败返回-1
int fd_pipe(int *); /* {Prog sock_fdpipe} */

// 接收一个文件描述符通过管道发送
// 参数: int fd - 管道文件描述符, ssize_t (*func)(int, const void *, size_t) - 处理接收数据的函数指针
// 返回: 接收到的文件描述符
int recv_fd(int, ssize_t (*func)(int, const void *, size_t)); /* {Prog recvfd_sockets} */

// 发送一个文件描述符通过管道
// 参数: int fd - 文件描述符, int fd_to_send - 要发送的文件描述符
// 返回: 成功返回0,失败返回-1
int send_fd(int, int); /* {Prog sendfd_sockets} */

// 发送错误信息
// 参数: int fd - 文件描述符, int errnum - 错误编号, const char *msg - 错误信息
// 返回: 成功返回0,失败返回-1
int send_err(int, int, const char *); /* {Prog senderr} */

// 服务器监听指定地址
// 参数: const char *addr - 监听地址
// 返回: 成功返回监听套接字文件描述符,失败返回-1
int serv_listen(const char *); /* {Prog servlisten_sockets} */

// 服务器接受客户端连接
// 参数: int listenfd - 监听套接字文件描述符, uid_t *uidptr - 存储客户端用户ID的指针
// 返回: 成功返回新的套接字文件描述符,失败返回-1
int serv_accept(int, uid_t *); /* {Prog servaccept_sockets} */

// 客户端连接到服务器
// 参数: const char *name - 服务器地址
// 返回: 成功返回套接字文件描述符,失败返回-1
int cli_conn(const char *); /* {Prog cliconn_sockets} */

// 从缓冲区解析命令行参数
// 参数: char *buf - 包含命令行参数的缓冲区, int (*func)(int, char **) - 处理参数的函数指针
// 返回: 成功返回参数数量,失败返回-1
int buf_args(char *, int (*func)(int, char **)); /* {Prog bufargs} */

// tty_cbreak函数用于将终端设置为cbreak模式,这种模式下,输入不会被回显,且输入的字符立即被传递给程序,不需要按下回车键.
// 参数:fd - 终端文件描述符
// 返回值:成功返回0,失败返回-1
int tty_cbreak(int); /* {Prog raw} */

// tty_raw函数用于将终端设置为原始模式,在这种模式下,终端不会对输入的字符做任何处理,所有的输入都会直接传递给程序.
// 参数:fd - 终端文件描述符
// 返回值:成功返回0,失败返回-1
int tty_raw(int); /* {Prog raw} */

// tty_reset函数用于将终端重置为其原始状态,关闭任何特殊模式,并恢复终端的默认设置.
// 参数:fd - 终端文件描述符
// 返回值:成功返回0,失败返回-1
int tty_reset(int); /* {Prog raw} */

// tty_atexit函数用于注册一个在程序退出时调用的函数,用于清理tty相关的资源.
// 参数:无
// 返回值:无
void tty_atexit(void); /* {Prog raw} */

// tty_termios函数用于获取当前终端的termios结构体指针,该结构体包含了控制终端行为的各种设置.
// 参数:无
// 返回值:指向termios结构体的指针
struct termios *tty_termios(void); /* {Prog raw} */

// ptym_open函数用于打开一个伪终端的主设备,通常用于创建一个子进程并与之通信.
// 参数:name - 伪终端主设备的名称;flags - 打开文件时使用的标志
// 返回值:成功返回文件描述符,失败返回-1
int ptym_open(char *, int); /* {Prog ptyopen} */

// ptys_open函数用于打开一个伪终端的从设备,通常与ptym_open配合使用,用于创建一个子进程并与之通信.
// 参数:name - 伪终端从设备的名称
// 返回值:成功返回文件描述符,失败返回-1
int ptys_open(char *); /* {Prog ptyopen} */

// #ifdef TIOCGWINSZ
// pty_fork函数用于创建一个子进程,并将其与一个伪终端关联.该函数会设置子进程的输入输出流,并返回子进程的PID.
// 参数:fdm - 指向文件描述符数组的指针,用于存储主伪终端和从伪终端的文件描述符;name - 伪终端名称;flags - 打开伪终端时的标志;
//       termp - 指向termios结构体的指针,用于设置子进程的终端属性;winp - 指向winsize结构体的指针,用于设置窗口大小
// 返回值:成功返回子进程的PID,失败返回-1
pid_t pty_fork(int *, char *, int, const struct termios *, const struct winsize *); /* {Prog ptyfork} */
// #endif
// lock_reg函数用于对文件描述符fd指定的文件进行锁定或解锁操作
// 参数fd: 文件描述符
// 参数cmd: 锁定或解锁命令,F_SETLK用于非阻塞锁定或解锁,F_SETLKW用于阻塞锁定
// 参数type: 锁定类型,F_RDLCK为共享读锁,F_WRLCK为独占写锁,F_UNLCK为解锁
// 参数offset: 锁定的起始偏移量
// 参数whence: 偏移量的起始位置,SEEK_SET表示从文件开始位置,SEEK_CUR表示从当前位置,SEEK_END表示从文件末尾
// 参数len: 锁定的长度,0表示锁定到文件末尾
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

// read_lock宏定义用于对文件进行非阻塞的共享读锁操作
#define read_lock(fd, offset, whence, len)   lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))

// readw_lock宏定义用于对文件进行阻塞的共享读锁操作
#define readw_lock(fd, offset, whence, len)  lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))

// write_lock宏定义用于对文件进行非阻塞的独占写锁操作
#define write_lock(fd, offset, whence, len)  lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))

// writew_lock宏定义用于对文件进行阻塞的独占写锁操作
#define writew_lock(fd, offset, whence, len) lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))

// un_lock宏定义用于对文件进行解锁操作
#define un_lock(fd, offset, whence, len)     lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

// lock_test函数用于测试文件描述符fd指定的文件在指定偏移量和长度上是否可以进行锁定操作
// 参数fd: 文件描述符
// 参数type: 锁定类型,F_RDLCK为共享读锁,F_WRLCK为独占写锁
// 参数offset: 锁定的起始偏移量
// 参数whence: 偏移量的起始位置
// 参数len: 锁定的长度
// 返回值: 如果可以进行锁定操作返回0,否则返回持有锁的进程ID
pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len);

// is_read_lockable宏定义用于测试文件是否可以进行共享读锁操作
#define is_read_lockable(fd, offset, whence, len)  (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)

// is_write_lockable宏定义用于测试文件是否可以进行独占写锁操作
#define is_write_lockable(fd, offset, whence, len) (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

// err_msg: 打印错误信息,第一个参数是格式化字符串,后续参数用于格式化
// {App misc_source}: 表示该函数定义在misc_source文件中
void err_msg(const char *, ...);

// err_dump: 打印错误信息并转储核心,不返回
// __attribute__((noreturn)): 告诉编译器这个函数不会返回
void err_dump(const char *, ...) __attribute__((noreturn));

// err_quit: 打印错误信息并退出程序,不返回
void err_quit(const char *, ...) __attribute__((noreturn));

// err_cont: 继续打印错误信息,第一个参数是状态码,后续参数用于格式化
void err_cont(int, const char *, ...);

// err_exit: 打印错误信息并以指定状态码退出程序,不返回
void err_exit(int, const char *, ...) __attribute__((noreturn));

// err_ret: 打印错误信息并返回
void err_ret(const char *, ...);

// err_sys: 打印系统错误信息并退出程序,不返回
void err_sys(const char *, ...) __attribute__((noreturn));

// 记录普通消息的函数,接受可变数量的参数
// 参数: msg - 要记录的消息字符串
void log_msg(const char *, ...); /* {App misc_source} */

// 打开日志系统的函数,设置日志级别和日志文件描述符
// 参数: path - 日志文件路径
//       level - 日志级别
//       fd - 日志文件描述符
void log_open(const char *, int, int);

// 记录退出消息的函数,不会返回
// 参数: msg - 要记录的退出消息字符串
void log_quit(const char *, ...) __attribute__((noreturn));

// 记录返回值的函数,接受可变数量的参数
// 参数: msg - 要记录的消息字符串
//       status - 返回的状态值
void log_ret(const char *, ...);

// 记录系统错误的函数,不会返回
// 参数: msg - 要记录的系统错误消息字符串
void log_sys(const char *, ...) __attribute__((noreturn));

// 记录退出状态的函数,不会返回
// 参数: status - 退出状态码
//       msg - 要记录的退出消息字符串
void log_exit(int, const char *, ...) __attribute__((noreturn));

/**
 * @brief 父子进程同步等待函数,用于防止竞态条件
 *
 */
void TELL_WAIT(void); /* parent/child from {Sec race_conditions} */

/**
 * @brief 父进程通知子进程继续执行
 * @param pid 子进程的进程ID
 */
void TELL_PARENT(pid_t);

/**
 * @brief 子进程通知父进程继续执行
 * @param pid 父进程的进程ID
 */
void TELL_CHILD(pid_t);

/**
 * @brief 父进程等待子进程完成
 */
void WAIT_PARENT(void);

/**
 * @brief 子进程等待父进程完成
 */
void WAIT_CHILD(void);

#endif /* _APUE_H */
