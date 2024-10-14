/*
 * Print server daemon.
 * 打印服务器守护进程.
 */

// 引入apue库的头文件,该库提供了一套用于编写Unix/Linux应用程序的API.
#include "apue.h"

// 引入文件控制相关的头文件,用于文件操作.
#include <fcntl.h>

// 引入目录操作相关的头文件,用于访问和操作目录.
#include <dirent.h>

// 引入字符处理相关的头文件,提供字符类型判断等功能.
#include <ctype.h>

// 引入用户信息相关的头文件,用于获取用户信息.
#include <pwd.h>

// 引入线程相关的头文件,用于多线程编程.
#include <pthread.h>

// 引入字符串处理相关的头文件,提供字符串比较等功能.
#include <strings.h>

// 引入select函数相关的头文件,用于I/O多路复用.
#include <sys/select.h>

// 引入系统I/O向量相关的头文件,用于高效的I/O操作.
#include <sys/uio.h>

// 引入自定义的print.h头文件,可能包含打印服务相关的定义和声明.
#include "print.h"

// 引入IPP(Internet Printing Protocol)相关的头文件,用于网络打印服务.
#include "ipp.h"

/*
 * 这些是针对打印机HTTP响应的定义.
 */
#define HTTP_INFO(x)    ((x) >= 100 && (x) <= 199)  // 定义HTTP信息响应状态码范围
#define HTTP_SUCCESS(x) ((x) >= 200 && (x) <= 299)  // 定义HTTP成功响应状态码范围

/*
 * 描述一个打印作业的结构体.
 */
struct job
{
    struct job     *next; /* 指向下一个作业的指针 */  // 链表中的下一个作业
    struct job     *prev; /* 指向前一个作业的指针 */  // 链表中的前一个作业
    int32_t         jobid; /* 作业ID */               // 打印作业的唯一标识符
    struct printreq req; /* 打印请求的副本 */         // 存储打印请求的详细信息
};
/*
 * 描述处理客户端请求的线程.
 */
struct worker_thread
{
    struct worker_thread *next;    // 链表中的下一个
    struct worker_thread *prev;    // 链表中的上一个
    pthread_t             tid;     // 线程ID
    int                   sockfd;  // 套接字
};
/*
 * 用于记录日志的变量.
 * 如果log_to_stderr被设置为1,则错误消息将被打印到标准错误流.
 */
int log_to_stderr = 0;

/*
 * 与打印机相关的结构和变量.
 * printer: 存储打印机地址信息的指针.
 * printer_name: 打印机的名称.
 * configlock: 用于保护配置信息的互斥锁.
 * reread: 标记是否需要重新读取配置文件.
 */
// 定义一个addrinfo结构体指针,用于存储网络地址信息
struct addrinfo *printer;

// 定义一个字符指针,用于存储打印机的名称
char *printer_name;

// 初始化一个互斥锁,用于保护共享资源的访问
// PTHREAD_MUTEX_INITIALIZER是pthread库提供的宏,用于静态初始化互斥锁
pthread_mutex_t configlock = PTHREAD_MUTEX_INITIALIZER;

// 定义一个整型变量,用于指示是否需要重新读取配置
int reread;
/*
 * 线程相关的结构和变量.
 * workers: 指向工作线程结构体数组的指针.
 * workerlock: 用于保护对工作线程数组访问的互斥锁.
 * mask: 用于线程信号屏蔽的集合.
 */
// 定义一个工作线程的指针数组
struct worker_thread *workers;

// 初始化一个互斥锁,用于保护对共享资源的访问
// PTHREAD_MUTEX_INITIALIZER 是一个宏,用于静态初始化互斥锁
pthread_mutex_t workerlock = PTHREAD_MUTEX_INITIALIZER;

// 定义一个信号集,用于存储被阻塞的信号
sigset_t mask;

/*
 * 任务相关的结构和变量.
 * jobhead, jobtail: 分别指向任务队列的头和尾.
 * jobfd: 用于任务通知的文件描述符.
 * nextjob: 下一个要处理的任务的标识符.
 * joblock: 用于保护对任务队列访问的互斥锁.
 * jobwait: 条件变量,工作线程在此等待新任务.
 */
// 定义作业结构体指针,分别指向作业队列的头和尾
struct job *jobhead, *jobtail;

// 定义文件描述符,用于作业的输入输出
int jobfd;

// 定义下一个作业的标识符
int32_t nextjob;

// 初始化互斥锁,用于保护作业队列的并发访问
pthread_mutex_t joblock = PTHREAD_MUTEX_INITIALIZER;

// 初始化条件变量,用于线程间的作业通知机制
pthread_cond_t jobwait = PTHREAD_COND_INITIALIZER;

/*
 * 函数原型声明.
 */

// 初始化请求队列
void init_request(void);

// 初始化打印机
void init_printer(void);

// 更新作业编号
void update_jobno(void);

// 获取新的作业编号
int32_t get_newjobno(void);

// 添加新作业到队列
// 参数:job - 指向打印请求结构的指针;jobno - 作业编号
void add_job(struct printreq *, int32_t);

// 替换队列中的作业
// 参数:job - 指向要替换的作业结构的指针
void replace_job(struct job *);

// 从队列中移除作业
// 参数:job - 指向要移除的作业结构的指针
void remove_job(struct job *);

// 系统启动时构建初始队列
void build_qonstart(void);

// 客户端线程函数
// 参数:arg - 线程参数
void *client_thread(void *);

// 打印机线程函数
// 参数:arg - 线程参数
void *printer_thread(void *);

// 信号线程函数
// 参数:arg - 线程参数
void *signal_thread(void *);

// 从文件描述符读取更多数据
// 参数:fd - 文件描述符;line - 指向存储读取数据的字符指针数组;nbytes - 要读取的字节数;eof - 指向整数的指针,用于标记是否到达文件末尾
ssize_t readmore(int, char **, int, int *);

// 获取打印机状态
// 参数:fd - 打印机文件描述符;job - 指向作业结构的指针
int printer_status(int, struct job *);

// 添加工作线程
// 参数:tid - 线程ID;pid - 进程ID
void add_worker(pthread_t, int);

// 终止所有工作线程
void kill_workers(void);

// 客户端清理函数
// 参数:arg - 清理参数
void client_cleanup(void *);

/*
 * Main print server thread.  Accepts connect requests from
 * clients and spawns additional threads to service requests.
 *
 * LOCKING: none.
 */

/*
 * 主打印服务器线程.接受来自客户端的连接请求,并生成额外的线程来处理请求.
 *
 * 锁定:无.
 */

// 以下代码段没有具体的函数方法,因此无需添加标准文档注释.
// 如果后续有具体的函数方法,请为其添加相应的注释.

int main(int argc, char *argv[])
{
    // 定义线程标识符
    pthread_t tid;

    // 定义地址信息列表和当前地址信息指针
    struct addrinfo *ailist, *aip;

    // 定义套接字文件描述符,错误码,循环变量,最大文件描述符
    int sockfd, err, i, n, maxfd;

    // 定义主机名指针
    char *host;

    // 定义用于select函数的文件描述符集合
    fd_set rendezvous, rset;

    // 定义信号处理结构体
    struct sigaction sa;

    // 定义密码结构体指针
    struct passwd *pwdp;

    // printd.c

    // 检查命令行参数的数量,如果不等于1,则打印使用方法并退出程序
    if (argc != 1)
        err_quit("usage: printd");

    // 将程序转为守护进程运行
    daemonize("printd");

    // 清空信号集
    sigemptyset(&sa.sa_mask);

    // 设置信号处理函数的标志位
    sa.sa_flags = 0;

    // 设置SIGPIPE信号的处理函数为忽略
    sa.sa_handler = SIG_IGN;

    // 设置SIGPIPE信号的处理函数,如果失败则记录系统错误并退出
    if (sigaction(SIGPIPE, &sa, NULL) < 0)
        log_sys("sigaction failed");

    // 清空信号掩码集
    sigemptyset(&mask);

    // 向信号掩码集中添加SIGHUP信号
    sigaddset(&mask, SIGHUP);

    // 向信号掩码集中添加SIGTERM信号
    sigaddset(&mask, SIGTERM);

    // 阻塞SIGHUP和SIGTERM信号,如果操作失败则记录系统错误并退出
    // 阻塞指定的信号集
    // 如果pthread_sigmask函数调用失败,则记录系统错误日志
    if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
        log_sys("pthread_sigmask failed");

    // 获取主机名的最大长度
    n = sysconf(_SC_HOST_NAME_MAX);
    // 如果获取失败,则使用默认的最大长度
    if (n < 0) /* best guess */
        n = HOST_NAME_MAX;
    // 分配内存空间以存储主机名
    if ((host = malloc(n)) == NULL)
        log_sys("malloc error");
    // 获取主机名
    // 获取当前主机的主机名
    // 如果获取失败,记录系统错误日志
    if (gethostname(host, n) < 0)
        log_sys("gethostname error");

    // 获取主机的地址信息列表
    // 如果获取地址信息失败,则记录错误并退出程序
    // 函数:getaddrlist
    // 参数:host - 主机名, "print" - 服务类型, &ailist - 地址信息列表的指针
    // 返回值:err - 错误码
    // 如果err不为0,表示获取地址信息失败,使用gai_strerror将错误码转换为字符串并记录错误,然后退出程序
    if ((err = getaddrlist(host, "print", &ailist)) != 0)
    {
        // 当getaddrinfo函数发生错误时,使用log_quit记录错误信息
        // gai_strerror(err)将错误码转换为可读的字符串
        // 然后调用exit(1)退出程序,1表示程序异常退出
        log_quit("getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    // 初始化服务器套接字集合
    FD_ZERO(&rendezvous);

    // 初始化最大文件描述符为-1
    maxfd = -1;

    // 遍历地址信息列表
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 初始化服务器套接字,如果成功则返回文件描述符
        // 如果成功初始化服务器套接字,则sockfd的值将大于或等于0
        // initserver函数用于初始化服务器套接字,参数包括套接字类型,地址信息,地址长度和队列长度
        // SOCK_STREAM表示使用TCP协议
        // aip->ai_addr是服务器地址
        // aip->ai_addrlen是服务器地址长度
        // QLEN是监听队列的最大长度
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
        {
            // 将新的文件描述符添加到集合中
            FD_SET(sockfd, &rendezvous);

            // 更新最大文件描述符
            // 如果当前套接字文件描述符大于最大文件描述符
            if (sockfd > maxfd)
                // 更新最大文件描述符为当前套接字文件描述符
                maxfd = sockfd;
        }
    }

    // 检查最大文件描述符是否为-1,如果是,则记录服务未启用并退出
    if (maxfd == -1)
        log_quit("service not enabled");

    // 获取用户名为LPNAME的用户信息
    pwdp = getpwnam(LPNAME);
    // 如果用户信息为空,记录找不到用户LPNAME并退出
    if (pwdp == NULL)
        log_sys("can't find user %s", LPNAME);
    // 如果用户的UID为0,表示用户是特权用户,记录用户LPNAME是特权用户并退出
    if (pwdp->pw_uid == 0)
        log_quit("user %s is privileged", LPNAME);
    // 尝试将进程的组ID和用户ID更改为用户LPNAME的组ID和用户ID,如果失败则记录错误并退出
    if (setgid(pwdp->pw_gid) < 0 || setuid(pwdp->pw_uid) < 0)
        log_sys("can't change IDs to user %s", LPNAME);

    // 初始化请求处理
    init_request();
    // 初始化打印机
    init_printer();

    // 创建打印机线程,如果成功则创建信号线程
    err = pthread_create(&tid, NULL, printer_thread, NULL);
    // 如果创建线程没有错误,则尝试创建信号线程
    if (err == 0)
        err = pthread_create(&tid, NULL, signal_thread, NULL);
    // 如果创建线程出错,记录错误并退出
    if (err != 0)
        log_exit(err, "can't create thread");

    // 构建启动时的队列
    build_qonstart();

    // 记录守护进程初始化完成
    log_msg("daemon initialized");

    // 无限循环,持续监听文件描述符集合
    for (;;)
    {
        // 将rendezvous赋值给rset,rendezvous是一个文件描述符集合
        rset = rendezvous;

        // 使用select函数监控文件描述符集合,maxfd + 1是监控的最大文件描述符加一
        // 如果select函数返回值小于0,表示出错,调用log_sys函数记录系统错误日志
        if (select(maxfd + 1, &rset, NULL, NULL, NULL) < 0)
            log_sys("select failed");

        // 遍历所有文件描述符,检查是否有准备好进行读操作的文件描述符
        for (i = 0; i <= maxfd; i++)
        {
            // 如果文件描述符i已经准备好进行读操作
            if (FD_ISSET(i, &rset))
            {
                /*
                 * 接受连接并处理请求.
                 */
                // 尝试接受新的连接,如果失败则记录错误并返回
                if ((sockfd = accept(i, NULL, NULL)) < 0)
                    log_ret("accept failed");
                // 创建一个新的线程来处理客户端请求,传递socket文件描述符作为参数
                // 创建一个新的线程,该线程将执行client_thread函数
                // tid: 线程标识符,用于引用新创建的线程
                // NULL: 指定线程属性,这里使用默认属性
                // client_thread: 新线程将要执行的函数
                // (void *)((long)sockfd): 传递给client_thread函数的参数,这里是将sockfd强制转换为(void *)类型的指针
                pthread_create(&tid, NULL, client_thread, (void *)((long)sockfd));
            }
        }
    }

    exit(1);
}

/*
 * 初始化作业ID文件.使用记录锁以防止多个打印守护进程同时运行.
 *
 * 锁定:除了对作业ID文件的记录锁之外,没有其他锁定.
 */
void init_request(void)
{
    int  n;               // 用于存储读取的字节数
    char name[FILENMSZ];  // 用于存储作业ID文件的路径

    // 构造作业ID文件的完整路径
    sprintf(name, "%s/%s", SPOOLDIR, JOBFILE);
    // 打开作业ID文件,如果不存在则创建,同时设置读写权限
    jobfd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // 如果无法获取文件锁,说明已经有守护进程在运行
    if (write_lock(jobfd, 0, SEEK_SET, 0) < 0)
        log_quit("daemon already running");

    /*
     * 重用name缓冲区作为作业计数器.
     */
    // 尝试读取作业ID文件中的内容
    if ((n = read(jobfd, name, FILENMSZ)) < 0)
        log_sys("can't read job file");  // 读取失败时记录系统错误
    // 如果文件为空,下一个作业ID为1
    if (n == 0)
        nextjob = 1;
    else
        // 否则将读取的内容转换为长整型作为下一个作业ID
        nextjob = atol(name);
}

/*
 * 从配置文件初始化打印机信息.
 *
 * 锁定:无.
 */
void init_printer(void)
{
    // 获取打印机地址
    printer = get_printaddr();
    // 如果打印机地址为空,则退出程序
    if (printer == NULL)
        exit(1); /* 消息已记录 */
    // 获取打印机的规范名称
    printer_name = printer->ai_canonname;
    // 如果打印机名称为空,则设置默认名称
    if (printer_name == NULL)
        printer_name = "printer";
    // 记录打印机名称
    log_msg("printer is %s", printer_name);
}

/*
 * 更新作业ID文件,写入下一个作业编号.
 * 不处理作业编号的回绕.
 *
 * 锁定:无.
 */

// 函数:update_jobno
// 功能:更新作业编号文件
// 参数:无
// 返回值:无
void update_jobno(void)
{
    char buf[32];  // 定义一个字符数组用于存储格式化后的作业编号

    // 尝试将文件指针移动到文件开头
    if (lseek(jobfd, 0, SEEK_SET) == -1)
        log_sys("can't seek in job file");  // 如果失败,记录系统错误

    // 将下一个作业编号格式化为字符串并存储到buf中
    sprintf(buf, "%d", nextjob);

    // 将buf中的内容写入作业编号文件
    if (write(jobfd, buf, strlen(buf)) < 0)
        log_sys("can't update job file");  // 如果写入失败,记录系统错误
}

/*
 * 获取下一个作业编号.
 *
 * 锁定:获取并释放joblock.
 */
int32_t
get_newjobno(void)
{
    int32_t jobid;  // 定义作业编号变量

    pthread_mutex_lock(&joblock);    // 加锁,保证线程安全
    jobid = nextjob++;               // 获取当前作业编号并递增nextjob
    if (nextjob <= 0)                // 如果nextjob小于等于0
        nextjob = 1;                 // 重置nextjob为1
    pthread_mutex_unlock(&joblock);  // 解锁
    return (jobid);                  // 返回作业编号
}

/*
 * 向待处理作业列表中添加新作业.然后通知打印机线程有作业待处理.
 *
 * 锁定:获取并释放joblock.
 */
void add_job(struct printreq *reqp, int32_t jobid)
{
    struct job *jp;  // 定义一个新的作业结构体指针

    // 分配内存空间给新的作业结构体
    if ((jp = malloc(sizeof(struct job))) == NULL)
        log_sys("malloc failed");  // 如果内存分配失败,记录系统日志

    // 将传入的打印请求结构体复制到新的作业结构体中
    memcpy(&jp->req, reqp, sizeof(struct printreq));
    jp->jobid = jobid;  // 设置作业ID
    jp->next  = NULL;   // 新作业的下一个指针初始化为NULL

    // 加锁,保证线程安全
    pthread_mutex_lock(&joblock);
    jp->prev = jobtail;  // 新作业的前一个指针指向当前的作业队列尾部

    // 如果作业队列为空,则新作业成为队列头部
    if (jobtail == NULL)
        jobhead = jp;
    else
        jobtail->next = jp;  // 否则,当前队列尾部的下一个指针指向新作业

    jobtail = jp;                    // 更新作业队列尾部为新作业
    pthread_mutex_unlock(&joblock);  // 解锁

    pthread_cond_signal(&jobwait);  // 通知等待的打印机线程有新作业到来
}

/*
 * 将一个作业重新放回链表头部.
 *
 * 锁定:获取并释放joblock.
 */
void replace_job(struct job *jp)
{
    pthread_mutex_lock(&joblock);  // 加锁,保证线程安全
    jp->prev = NULL;               // 将作业的前一个节点设置为NULL
    jp->next = jobhead;            // 将作业的下一个节点设置为当前链表头
    if (jobhead == NULL)           // 如果链表为空
        jobtail = jp;              // 则作业既是头也是尾
    else
        jobhead->prev = jp;          // 否则,将原链表头的prev指向当前作业
    jobhead = jp;                    // 更新链表头为当前作业
    pthread_mutex_unlock(&joblock);  // 解锁
}

/*
 * 从待处理作业列表中移除一个作业.
 *
 * 锁定:调用者必须持有joblock.
 */
void remove_job(struct job *target)
{
    // 如果目标作业有下一个作业,则将下一个作业的前一个作业设置为目标作业的前一个作业
    if (target->next != NULL)
        target->next->prev = target->prev;
    else
        // 如果没有下一个作业,说明目标作业是列表中的最后一个作业,更新jobtail
        jobtail = target->prev;
    // 如果目标作业有前一个作业,则将前一个作业的下一个作业设置为目标作业的下一个作业
    if (target->prev != NULL)
        target->prev->next = target->next;
    else
        // 如果没有前一个作业,说明目标作业是列表中的第一个作业,更新jobhead
        jobhead = target->next;
}

/*
 * 在启动时检查待处理的打印作业.
 *
 * 锁定:无.
 */
void build_qonstart(void)
{
    int             fd, err, nr;                       // 文件描述符,错误码,读取的字节数
    int32_t         jobid;                             // 作业ID
    DIR            *dirp;                              // 目录指针
    struct dirent  *entp;                              // 目录项指针
    struct printreq req;                               // 打印请求结构体
    char            dname[FILENMSZ], fname[FILENMSZ];  // 目录名和文件名缓冲区

    // 构造待检查的目录路径
    sprintf(dname, "%s/%s", SPOOLDIR, REQDIR);
    // 打开目录
    // 打开目录,如果失败则返回
    // 参数:
    //   dname - 要打开的目录名
    // 返回值:
    //   如果成功,返回目录流指针;如果失败,返回NULL
    if ((dirp = opendir(dname)) == NULL)
        return;

    // 遍历目录中的每个条目
    while ((entp = readdir(dirp)) != NULL)
    {
        // 跳过"."和".."这两个特殊目录项
        // 跳过当前循环,如果当前目录项的名称是"."或者"..",这两个分别代表当前目录和父目录
        if (strcmp(entp->d_name, ".") == 0 ||  // 比较当前目录项的名称是否为"."
            strcmp(entp->d_name, "..") == 0)   // 比较当前目录项的名称是否为".."
            continue;                          // 如果是,则跳过本次循环

        // 构造完整的文件路径
        sprintf(fname, "%s/%s/%s", SPOOLDIR, REQDIR, entp->d_name);
        // 打开文件
        if ((fd = open(fname, O_RDONLY)) < 0)
            continue;
        // 读取打印请求结构体
        nr = read(fd, &req, sizeof(struct printreq));
        // 检查是否成功读取了完整的结构体
        // 检查读取的字节数是否与结构体大小相等
        if (nr != sizeof(struct printreq))
        {
            // 处理读取错误
            // 如果nr小于0,表示系统调用出错,记录系统错误号
            if (nr < 0)
                err = errno;
            else
                // 否则,设置err为EIO,表示输入输出错误
                err = EIO;
            // 关闭文件描述符
            close(fd);
            // 记录错误日志并删除文件
            log_msg("build_qonstart: can't read %s: %s",
                    fname,
                    strerror(err));
            // 删除原文件
            unlink(fname);
            // 构造数据文件的路径并删除对应的数据文件
            // 将拼接后的文件路径存储到fname中
            // SPOOLDIR和DATADIR是预定义的目录路径,entp->d_name是当前处理的文件名
            sprintf(fname, "%s/%s/%s", SPOOLDIR, DATADIR, entp->d_name);

            // 删除拼接后得到的文件路径所指向的文件
            unlink(fname);

            // 继续下一次循环
            continue;
        }

        // 将目录项名称转换为作业ID
        jobid = atol(entp->d_name);
        // 记录日志,表示将作业添加到队列
        log_msg("adding job %d to queue", jobid);
        // 添加作业到队列
        add_job(&req, jobid);
    }
    // 关闭目录
    // 关闭目录流
    // 参数:
    //   dirp - 指向要关闭的目录流的指针
    // 返回值:
    //   成功返回0,失败返回-1并设置errno
    closedir(dirp);
}

/*
 * Accept a print job from a client.
 *
 * LOCKING: none.
 */

/*
 * 接受来自客户端的打印任务.
 *
 * 锁定:无.
 */

void *
client_thread(void *arg)
{
    // 定义变量n,用于存储整数值
    int n;
    // 定义文件描述符fd
    int fd;
    // 定义套接字描述符sockfd
    int sockfd;
    // 定义变量nr和nw,分别用于存储读取和写入的字节数
    int nr, nw;
    // 定义布尔变量first,用于标记是否是第一次操作
    int first;
    // 定义32位整型变量jobid,可能用于标识打印任务
    int32_t jobid;
    // 定义线程ID变量tid,用于多线程编程
    pthread_t tid;
    // 定义结构体printreq类型的变量req,可能用于存储打印请求信息
    struct printreq req;
    // 定义结构体printresp类型的变量res,可能用于存储打印响应信息
    struct printresp res;
    // 定义字符数组name,大小为FILENMSZ,用于存储文件名
    char name[FILENMSZ];
    // 定义字符数组buf,大小为IOBUFSZ,用于存储输入输出缓冲区数据
    char buf[IOBUFSZ];

    // 获取当前线程的ID
    tid = pthread_self();

    // 注册清理函数,当线程退出时调用.client_cleanup是清理函数,(void *)((long)tid)是传递给清理函数的参数
    pthread_cleanup_push(client_cleanup, (void *)((long)tid));

    // 将参数arg转换为long类型并赋值给sockfd,arg应该是之前传递给线程的socket文件描述符
    sockfd = (long)arg;

    // 将线程ID和socket文件描述符添加到工作线程列表中
    add_worker(tid, sockfd);

    /*
     * 读取请求头.
     */
    // 尝试从套接字读取请求头,最多等待10秒
    // 从文件: printd.c
    // 检查从套接字读取的数据大小是否与printreq结构体的大小相匹配
    // 如果不匹配,可能表示读取过程中出现了错误或数据不完整
    if ((n = treadn(sockfd, &req, sizeof(struct printreq), 10)) != sizeof(struct printreq))

    {
        // 如果读取的字节数不等于请求头的大小,则设置响应的jobid为0
        res.jobid = 0;
        // 如果n小于0,说明读取过程中发生了错误,将errno转化为网络字节序并赋值给retcode
        if (n < 0)
            res.retcode = htonl(errno);
        else
            // 否则,设置retcode为EIO错误
            res.retcode = htonl(EIO);
        // 将retcode对应的错误信息复制到响应消息中
        strncpy(res.msg, strerror(res.retcode), MSGLEN_MAX);
        // 将响应结构体写入套接字
        writen(sockfd, &res, sizeof(struct printresp));
        // 退出线程
        pthread_exit((void *)1);
    }
    // 将请求结构体中的size和flags字段从网络字节序转换为主机字节序
    // 将网络字节序的整数值转换为主机字节序
    // req.size 和 req.flags 是请求结构体中的字段,分别表示请求的大小和标志
    // ntohl() 函数用于将32位无符号整数从网络字节序转换为主机字节序
    req.size  = ntohl(req.size);   // 转换请求大小为主机字节序
    req.flags = ntohl(req.flags);  // 转换请求标志为主机字节序

    /*
     * 创建数据文件.
     */
    // 获取一个新的作业编号
    jobid = get_newjobno();
    // 格式化文件路径
    sprintf(name, "%s/%s/%d", SPOOLDIR, DATADIR, jobid);
    // 创建文件,FILEPERM是文件权限
    fd = creat(name, FILEPERM);
    // 检查文件是否成功创建
    if (fd < 0)
    {
        // 设置响应结构体中的作业编号为0,表示失败
        res.jobid = 0;
        // 将errno转换为网络字节序并赋值给响应结构体中的返回码
        res.retcode = htonl(errno);
        // 记录错误日志,包含文件名和错误描述
        log_msg("client_thread: can't create %s: %s", name, strerror(res.retcode));
        // 将错误描述复制到响应结构体的消息字段中
        strncpy(res.msg, strerror(res.retcode), MSGLEN_MAX);
        // 将响应结构体写入套接字
        writen(sockfd, &res, sizeof(struct printresp));
        // 终止线程
        pthread_exit((void *)1);
    }

    /*
     * Read the file and store it in the spool directory.
     * Try to figure out if the file is a PostScript file
     * or a plain text file.
     */
    // 定义变量first并初始化为1,用于标记是否是第一次读取数据
    first = 1;

    // 使用while循环读取sockfd中的数据到buf中,直到读取的字节数不大于0
    while ((nr = tread(sockfd, buf, IOBUFSZ, 20)) > 0)
    {
        // 如果是第一次读取数据
        if (first)
        {
            first = 0;  // 将first设置为0,表示已经不是第一次读取
            // 检查buf中的前4个字符是否为"%!PS",如果不是,则设置req.flags的PR_TEXT位
            if (strncmp(buf, "%!PS", 4) != 0)
                req.flags |= PR_TEXT;
        }
        // 将buf中的数据写入到fd中,写入的字节数为nr
        nw = write(fd, buf, nr);
        // 如果写入的字节数与读取的字节数不相等
        if (nw != nr)
        {
            res.jobid = 0;  // 设置res.jobid为0
            // 如果写入的字节数小于0,将errno的值转换为网络字节序并赋值给res.retcode
            // 否则,设置res.retcode为EIO的网络字节序
            if (nw < 0)
                res.retcode = htonl(errno);
            else
                res.retcode = htonl(EIO);
            // 记录日志,包含客户端线程无法写入的信息和错误原因
            log_msg("client_thread: can't write %s: %s", name, strerror(res.retcode));
            close(fd);  // 关闭文件描述符fd
            // 将错误原因复制到res.msg中,最多复制MSGLEN_MAX个字符
            strncpy(res.msg, strerror(res.retcode), MSGLEN_MAX);
            // 将res结构体写入到sockfd中
            // 将结果结构体写入套接字
            // 参数:
            //   sockfd: 套接字文件描述符
            //   res: 指向要发送的结果结构体的指针
            //   sizeof(struct printresp): 结果结构体的大小
            writen(sockfd, &res, sizeof(struct printresp));

            unlink(name);             // 删除文件name
            pthread_exit((void *)1);  // 退出线程,返回值为1
        }
    }
    // 关闭文件描述符fd
    close(fd);

    /*
     * 创建控制文件.然后将打印请求信息写入控制文件.
     */
    // 格式化控制文件的路径
    sprintf(name, "%s/%s/%d", SPOOLDIR, REQDIR, jobid);
    // 创建控制文件
    fd = creat(name, FILEPERM);
    // 检查文件是否成功创建
    if (fd < 0)
    {
        // 设置响应结构体中的错误信息
        res.jobid   = 0;
        res.retcode = htonl(errno);
        // 记录错误日志
        log_msg("client_thread: can't create %s: %s", name, strerror(res.retcode));
        // 将错误信息复制到响应结构体中
        strncpy(res.msg, strerror(res.retcode), MSGLEN_MAX);
        // 将响应结构体发送回客户端
        writen(sockfd, &res, sizeof(struct printresp));
        // 如果控制文件创建失败,删除可能已创建的数据文件
        sprintf(name, "%s/%s/%d", SPOOLDIR, DATADIR, jobid);
        unlink(name);
        // 终止线程
        pthread_exit((void *)1);
    }
    // 将打印请求写入控制文件
    nw = write(fd, &req, sizeof(struct printreq));

    // 检查接收到的数据大小是否与printreq结构体的大小相匹配
    if (nw != sizeof(struct printreq))
    {
        // 如果不匹配,设置作业ID为0
        res.jobid = 0;
        // 如果接收到的数据大小小于0,将errno转化为网络字节序并赋值给retcode
        if (nw < 0)
            res.retcode = htonl(errno);
        else
            // 否则,设置retcode为EIO错误的网络字节序表示
            res.retcode = htonl(EIO);
        // 记录错误日志,包含客户端线程无法写入的原因
        log_msg("client_thread: can't write %s: %s", name, strerror(res.retcode));
        // 关闭文件描述符
        close(fd);
        // 将错误信息复制到响应消息中
        strncpy(res.msg, strerror(res.retcode), MSGLEN_MAX);
        // 将响应结构体写入套接字
        writen(sockfd, &res, sizeof(struct printresp));
        // 删除临时文件
        unlink(name);
        // 重新构造文件路径并删除
        sprintf(name, "%s/%s/%d", SPOOLDIR, DATADIR, jobid);
        unlink(name);
        // 线程退出,返回错误代码
        pthread_exit((void *)1);
    }
    // 如果数据大小匹配,正常关闭文件描述符
    close(fd);

    /*
     * 发送响应给客户端.
     *
     * @param sockfd: 客户端的套接字描述符
     * @param jobid: 请求的作业ID
     * @param res: 响应结构体,包含返回码,作业ID和消息
     *
     * 此函数设置响应结构体的返回码为0,将作业ID转换为网络字节序,并格式化消息,
     * 然后将响应结构体写入到客户端的套接字中.
     */
    res.retcode = 0;                                 // 设置返回码为0,表示成功
    res.jobid   = htonl(jobid);                      // 将作业ID转换为网络字节序
    sprintf(res.msg, "request ID %d", jobid);        // 格式化消息,包含请求的作业ID
    writen(sockfd, &res, sizeof(struct printresp));  // 将响应结构体写入客户端套接字

    /*
     * 通知打印机线程,清理并退出.
     *
     * @param jobid 要添加到队列的作业ID
     */
    log_msg("adding job %d to queue", jobid);  // 记录日志,表明正在将作业添加到队列
    add_job(&req, jobid);                      // 将作业添加到请求队列
    pthread_cleanup_pop(1);                    // 清理线程资源
    return ((void *)0);                        // 返回空指针,表示函数执行成功
}

/*
 * 向工作线程列表中添加一个工作线程.
 *
 * 锁定:获取并释放workerlock.
 */
void add_worker(pthread_t tid, int sockfd)
{
    // 定义一个指向工作线程结构体的指针
    struct worker_thread *wtp;

    // 分配内存空间给工作线程结构体,如果分配失败则记录错误并退出线程
    if ((wtp = malloc(sizeof(struct worker_thread))) == NULL)
    {
        log_ret("add_worker: can't malloc");
        pthread_exit((void *)1);
    }
    // 设置工作线程的tid和sockfd
    wtp->tid    = tid;
    wtp->sockfd = sockfd;
    // 获取workerlock互斥锁
    pthread_mutex_lock(&workerlock);
    // 初始化新工作线程的前驱指针为NULL
    wtp->prev = NULL;
    // 新工作线程的后继指针指向当前工作线程列表的头节点
    wtp->next = workers;
    // 如果工作线程列表为空,则将新工作线程设置为头节点
    if (workers == NULL)
        workers = wtp;
    else
        // 否则,将当前头节点的前驱指针指向新工作线程
        workers->prev = wtp;
    // 释放workerlock互斥锁
    pthread_mutex_unlock(&workerlock);
}

/*
 * 取消(杀死)所有未完成的工作线程.
 *
 * 锁定:获取并释放工作线程锁.
 */
void kill_workers(void)
{
    struct worker_thread *wtp;  // 定义一个指向工作线程结构体的指针

    pthread_mutex_lock(&workerlock);                   // 加锁,确保在操作工作线程列表时不会有并发问题
    for (wtp = workers; wtp != NULL; wtp = wtp->next)  // 遍历工作线程列表
        pthread_cancel(wtp->tid);                      // 取消当前工作线程的执行
    pthread_mutex_unlock(&workerlock);                 // 解锁,允许其他线程访问工作线程列表
}

/*
 * 工作线程的取消例程.
 *
 * 锁定:获取并释放workerlock.
 */
void client_cleanup(void *arg)
{
    struct worker_thread *wtp;  // 定义一个指向worker_thread结构体的指针
    pthread_t             tid;  // 定义一个pthread_t类型的变量用于存储线程ID

    tid = (pthread_t)((long)arg);     // 将传入的参数转换为pthread_t类型并赋值给tid
    pthread_mutex_lock(&workerlock);  // 加锁workerlock,保证线程安全

    // 遍历workers链表,寻找与tid匹配的worker_thread
    for (wtp = workers; wtp != NULL; wtp = wtp->next)
    {
        if (wtp->tid == tid)
        {
            // 如果找到匹配的worker_thread,调整链表指针,移除该节点
            // 如果wtp的next节点不为空,则将wtp的next节点的prev指针指向wtp的prev节点
            if (wtp->next != NULL)
                wtp->next->prev = wtp->prev;
            // 如果wtp的prev节点不为空,则将wtp的prev节点的next指针指向wtp的next节点
            // 如果wtp的prev节点为空,说明wtp是头节点,需要更新workers指针指向wtp的下一个节点
            if (wtp->prev != NULL)
                wtp->prev->next = wtp->next;
            else
                workers = wtp->next;
            // 找到目标节点后跳出循环
            break;
        }
    }

    pthread_mutex_unlock(&workerlock);  // 解锁workerlock

    // 如果找到了对应的worker_thread,关闭其socket连接并释放内存
    // 如果wtp指针不为空,则执行以下操作:
    // 1. 关闭wtp结构体中的sockfd文件描述符
    // 2. 释放wtp指针所指向的内存空间
    if (wtp != NULL)
    {
        close(wtp->sockfd);  // 关闭文件描述符
        free(wtp);           // 释放内存
    }
}

/*
 * 处理信号.
 *
 * 锁定:获取并释放configlock.
 */

/**
 * @brief 信号处理线程函数
 *
 * @param arg 线程参数
 * @return void* 线程返回值
 */
void *
signal_thread(void *arg)
{

    // 声明变量err,用于存储错误代码
    int err;

    // 声明变量signo,用于存储信号编号
    int signo;

    // 无限循环等待信号
    for (;;)
    {
        // 等待信号,将接收到的信号存储在signo中
        err = sigwait(&mask, &signo);
        if (err != 0)
            // 如果sigwait失败,记录错误并退出
            log_quit("sigwait failed: %s", strerror(err));
        switch (signo)
        {
        case SIGHUP:
            // 收到SIGHUP信号,重新读取配置文件
            pthread_mutex_lock(&configlock);    // 加锁
            reread = 1;                         // 设置重新读取标志
            pthread_mutex_unlock(&configlock);  // 解锁
            break;

        case SIGTERM:
            // 收到SIGTERM信号,终止工作线程并退出程序
            // 中止所有工作线程
            kill_workers();

            // 记录终止信号信息
            // @param signo 接收到的信号编号
            // @return 无返回值,直接打印信号名称到日志
            log_msg("terminate with signal %s", strsignal(signo));

            // 退出程序
            exit(0);

        default:
            // 收到未处理的信号,终止工作线程并记录错误
            // 停止所有工作线程
            kill_workers();

            // 记录意外信号的日志,并退出程序
            // @param signo 捕获到的信号编号
            log_quit("unexpected signal %d", signo);
        }
    }
}

/*
 * 向IPP头部添加一个选项.
 *
 * 锁定:无.
 */
/*
 * 函数功能:向配置选项中添加一个新的选项
 * 参数:
 *   cp - 指向配置选项字符串的指针
 *   tag - 选项的标签
 *   optname - 选项的名称
 *   optval - 选项的值
 * 返回值:
 *   成功时返回指向新配置字符串的指针,失败时返回NULL
 */
char *
add_option(char *cp, int tag, char *optname, char *optval)
{
    int n;
    // 定义一个联合体,用于存储网络字节序的整数
    union
    {
        int16_t s;
        char    c[2];
    } u;

    // 将标签写入cp指向的位置,并将cp指针后移一位
    *cp++ = tag;
    // 获取选项名称的长度
    n = strlen(optname);
    // 将选项名称的长度转换为网络字节序,并存储在联合体u中
    u.s = htons(n);
    // 将网络字节序的高位和低位分别写入cp指向的位置,并将cp指针后移两位
    *cp++ = u.c[0];
    *cp++ = u.c[1];
    // 将选项名称复制到cp指向的位置,并将cp指针后移n位
    strcpy(cp, optname);
    cp += n;
    // 获取选项值的长度
    n = strlen(optval);
    // 将选项值的长度转换为网络字节序,并存储在联合体u中
    u.s = htons(n);
    // 将网络字节序的高位和低位分别写入cp指向的位置,并将cp指针后移两位
    *cp++ = u.c[0];
    *cp++ = u.c[1];
    // 将选项值复制到cp指向的位置
    strcpy(cp, optval);
    // 返回cp指针移动后的位置
    return (cp + n);
}

/*
 * Single thread to communicate with the printer.
 *
 * LOCKING: acquires and releases joblock and configlock.
 */

/*
 * 单线程与打印机通信.
 *
 * 锁定:获取并释放joblock和configlock.
 */

void *
printer_thread(void *arg)
{
    // 定义一个作业结构体指针
    struct job *jp;

    // 定义整型变量,用于存储各种长度和文件描述符等信息
    int hlen, ilen, sockfd, fd, nr, nw, extra;

    // 定义字符指针,用于指向缓冲区
    char *icp, *hcp, *p;

    // 定义IPP头部结构体指针
    struct ipp_hdr *hp;

    // 定义stat结构体,用于存储文件状态信息
    struct stat sbuf;

    // 定义iovec结构体数组,用于scatter/gather I/O操作
    struct iovec iov[2];

    // 定义字符数组,用于存储文件名
    char name[FILENMSZ];

    // 定义字符数组,用于存储HTTP头部信息
    char hbuf[HBUFSZ];

    // 定义字符数组,用于存储IPP请求信息
    char ibuf[IBUFSZ];

    // 定义字符数组,用于通用I/O操作
    char buf[IOBUFSZ];

    // 定义字符数组,用于格式化字符串
    char str[64];

    // 定义timespec结构体,设置超时时间为1分钟
    struct timespec ts = {60, 0};

    for (;;)
    {
        /*
         * 获取一个打印任务.
         */
        // 加锁,保证线程安全
        pthread_mutex_lock(&joblock);
        // 如果没有任务,则等待
        while (jobhead == NULL)
        {
            log_msg("printer_thread: 等待中...");
            // 线程等待条件变量,释放锁并阻塞,直到被唤醒
            pthread_cond_wait(&jobwait, &joblock);
        }
        // 移除队列头部的任务
        remove_job(jp = jobhead);
        // 记录日志,表示已获取到任务
        log_msg("printer_thread: 已拾取任务 %d", jp->jobid);
        // 解锁
        pthread_mutex_unlock(&joblock);
        // 更新任务编号
        update_jobno();

        /*
         * 检查配置文件是否有更改.
         */
        // 加锁,防止并发访问配置
        pthread_mutex_lock(&configlock);
        // 如果需要重新读取配置
        if (reread)
        {
            // 释放之前分配的地址信息
            freeaddrinfo(printer);
            // 将打印机信息置空
            printer      = NULL;
            printer_name = NULL;
            // 重置重新读取标志
            reread = 0;
            // 解锁配置锁
            pthread_mutex_unlock(&configlock);
            // 初始化打印机设置
            init_printer();
        }
        else
        {
            // 如果不需要重新读取配置,则直接解锁
            pthread_mutex_unlock(&configlock);
        }

        /*
         * 发送作业到打印机.
         * 使用sprintf函数构造要打开的文件名,格式为SPOOLDIR/DATADIR/jobid.
         */
        sprintf(name, "%s/%s/%d", SPOOLDIR, DATADIR, jp->jobid);

        /*
         * 尝试以只读模式打开文件.
         * 如果打开失败(返回值小于0),记录错误信息并释放jp指向的内存.
         */
        // 打开文件,如果失败则记录错误信息并释放作业结构体内存
        if ((fd = open(name, O_RDONLY)) < 0)  // 尝试以只读模式打开文件
        {
            log_msg("job %d canceled - can't open %s: %s",  // 记录错误日志
                    jp->jobid,                              // 作业ID
                    name,                                   // 文件名
                    strerror(errno));                       // 错误描述
            free(jp);                                       // 释放作业结构体内存
            continue;                                       // 跳过当前循环,继续下一次循环
        }

        // 检查文件状态,如果失败则记录错误信息并清理资源
        if (fstat(fd, &sbuf) < 0)
        {
            // 记录错误日志:作业ID,文件名,以及具体的错误信息
            log_msg("job %d canceled - can't fstat %s: %s",
                    jp->jobid,         // 作业ID
                    name,              // 文件名
                    strerror(errno));  // 错误信息
            free(jp);                  // 释放作业结构体内存
            close(fd);                 // 关闭文件描述符
            continue;                  // 跳过当前循环,继续下一次循环
        }

        // 连接到打印机的函数,如果连接失败,则记录错误信息并跳转到defer标签
        if ((sockfd = connect_retry(AF_INET, SOCK_STREAM, 0, printer->ai_addr, printer->ai_addrlen)) < 0)
        {
            // 记录无法联系到打印机的错误信息,包括作业ID和具体的错误描述
            log_msg("job %d deferred - can't contact printer: %s",
                    jp->jobid,
                    strerror(errno));
            // 跳转到defer标签,可能用于稍后重试或者其他错误处理
            goto defer;
        }

        /*
         * 设置IPP头部.
         */
        icp               = ibuf;                                                        // 将ibuf的地址赋给icp,ibuf是缓冲区的起始地址
        hp                = (struct ipp_hdr *)icp;                                       // 将icp强制转换为ipp_hdr结构体指针,赋给hp
        hp->major_version = 1;                                                           // 设置IPP协议的主要版本号为1
        hp->minor_version = 1;                                                           // 设置IPP协议的次要版本号为1
        hp->operation     = htons(OP_PRINT_JOB);                                         // 设置操作码为打印作业
        hp->request_id    = htonl(jp->jobid);                                            // 设置请求ID,将jp->jobid的值从主机字节序转换为网络字节序
        icp += offsetof(struct ipp_hdr, attr_group);                                     // 将icp指针移动到IPP头部属性组的起始位置
        *icp++ = TAG_OPERATION_ATTR;                                                     // 写入操作属性标签
        icp    = add_option(icp, TAG_CHARSET, "attributes-charset", "utf-8");            // 添加字符集选项
        icp    = add_option(icp, TAG_NATULANG, "attributes-natural-language", "en-us");  // 添加自然语言选项
        sprintf(str, "http://%s/ipp", printer_name);                                     // 格式化打印机URI字符串
        icp = add_option(icp, TAG_URI, "printer-uri", str);                              // 添加打印机URI选项
        icp = add_option(icp, TAG_NAMEWOLANG, "requesting-user-name", jp->req.usernm);   // 添加请求用户名选项
        icp = add_option(icp, TAG_NAMEWOLANG, "job-name", jp->req.jobnm);                // 添加作业名称选项

        // 检查jp->req.flags是否包含PR_TEXT标志
        if (jp->req.flags & PR_TEXT)
        {
            // 如果包含PR_TEXT标志,设置MIME类型为"text/plain",并增加extra计数
            p     = "text/plain";
            extra = 1;
        }
        else
        {
            // 如果不包含PR_TEXT标志,设置MIME类型为"application/postscript",extra计数保持为0
            p     = "application/postscript";
            extra = 0;
        }

        // 添加MIME类型选项到icp
        icp = add_option(icp, TAG_MIMETYPE, "document-format", p);
        // 标记属性结束
        *icp++ = TAG_END_OF_ATTR;
        // 计算icp和ibuf之间的长度
        ilen = icp - ibuf;

        /*
         * 设置HTTP头部.
         */
        // 初始化hcp指向hbuf
        hcp = hbuf;
        // 写入HTTP请求方法,路径和版本
        sprintf(hcp, "POST /ipp HTTP/1.1\r\n");
        hcp += strlen(hcp);
        // 写入内容长度,包括sbuf.st_size,ilen和额外空间
        sprintf(hcp, "Content-Length: %ld\r\n", (long)sbuf.st_size + ilen + extra);
        hcp += strlen(hcp);
        // 写入内容类型
        strcpy(hcp, "Content-Type: application/ipp\r\n");
        hcp += strlen(hcp);
        // 写入主机名和端口
        sprintf(hcp, "Host: %s:%d\r\n", printer_name, IPP_PORT);
        hcp += strlen(hcp);
        // 写入HTTP头部结束标志
        *hcp++ = '\r';
        *hcp++ = '\n';
        // 计算hcp和hbuf之间的长度,即HTTP头部长度
        hlen = hcp - hbuf;

        /*
         * 首先写入头部信息.然后发送文件.
         *
         * @param iov 一个指向iovec结构体数组的指针,用于存储要写入的数据块
         * @param sockfd 文件描述符,表示要写入的打印机
         * @param hbuf 指向头部信息缓冲区的指针
         * @param hlen 头部信息的长度
         * @param ibuf 指向实际要打印的数据缓冲区的指针
         * @param ilen 实际要打印的数据的长度
         * @return 如果写入成功,返回写入的字节数;如果失败,记录错误并跳转到defer标签
         */
        iov[0].iov_base = hbuf;                     // 设置头部信息缓冲区的地址
        iov[0].iov_len  = hlen;                     // 设置头部信息的长度
        iov[1].iov_base = ibuf;                     // 设置实际要打印的数据缓冲区的地址
        iov[1].iov_len  = ilen;                     // 设置实际要打印的数据的长度
        if (writev(sockfd, iov, 2) != hlen + ilen)  // 尝试写入头部信息和数据到打印机
        {
            log_ret("can't write to printer");  // 如果写入的字节数不等于头部信息和数据的总长度,记录错误
            goto defer;                         // 跳转到defer标签处理错误
        }

        // 检查请求标志是否包含 PR_TEXT 标志
        // 如果 jp->req.flags 与 PR_TEXT 进行位与操作的结果不为0,表示请求中包含文本标志
        if (jp->req.flags & PR_TEXT)
        {
            /*
             * 如果打印请求标志中包含 PR_TEXT,表示允许将 PostScript 作为纯文本打印.
             * 这是一种hack方法,通过发送退格符 '\b' 到打印机,尝试将 PostScript 转换为纯文本模式.
             */
            if (write(sockfd, "\b", 1) != 1)
            {
                /*
                 * 如果向打印机写入退格符失败,则记录错误并跳转到 defer 标签处,
                 * defer 标签可能包含处理错误的代码.
                 */
                log_ret("can't write to printer");
                goto defer;
            }
        }

        // 读取数据并写入打印机的循环
        while ((nr = read(fd, buf, IOBUFSZ)) > 0)
        {
            // 尝试将读取的数据写入打印机
            if ((nw = writen(sockfd, buf, nr)) != nr)
            {
                // 如果写入的字节数小于0,表示写入失败
                if (nw < 0)
                    log_ret("无法写入打印机");  // 记录错误并返回
                else
                    log_msg("写入打印机数据不足 (%d/%d)", nw, nr);  // 记录警告信息
                goto defer;                                         // 跳转到defer标签处
            }
        }

        // 如果读取的页数小于0,记录错误并跳转到defer标签
        if (nr < 0)
        {
            log_ret("无法读取 %s", name);
            goto defer;
        }

        /*
         * 从打印机读取响应.
         */
        // 检查打印机状态,如果状态异常
        if (printer_status(sockfd, jp))
        {
            // 删除临时文件
            unlink(name);
            // 重新构建文件路径
            sprintf(name, "%s/%s/%d", SPOOLDIR, REQDIR, jp->jobid);
            // 再次删除文件
            unlink(name);
            // 释放jp指针指向的内存
            free(jp);
            // 将jp指针置空
            jp = NULL;
        }

    // 使用defer关键字确保文件描述符fd在函数退出时关闭
    defer:
        close(fd);          // 关闭文件描述符fd
        if (sockfd >= 0)    // 如果sockfd有效
            close(sockfd);  // 关闭套接字文件描述符sockfd
        if (jp != NULL)     // 如果指针jp非空
        {
            replace_job(jp);       // 调用replace_job函数处理jp指向的工作
            nanosleep(&ts, NULL);  // 使当前线程休眠指定的时间
        }
    }
}

/*
 * 从打印机读取数据,可能需要增加缓冲区.
 * 返回缓冲区中数据结束位置的偏移量,或在失败时返回-1.
 *
 * 锁定:无.
 */
ssize_t
readmore(int sockfd, char **bpp, int off, int *bszp)
{
    ssize_t nr;           // 读取的字节数
    char   *bp  = *bpp;   // 指向缓冲区的指针
    int     bsz = *bszp;  // 缓冲区的大小

    // 如果当前偏移量超过了缓冲区大小,需要重新分配更大的缓冲区
    if (off >= bsz)
    {
        bsz += IOBUFSZ;  // 增加缓冲区大小
        // 尝试重新分配内存
        if ((bp = realloc(*bpp, bsz)) == NULL)
            log_sys("readmore: can't allocate bigger read buffer");  // 分配失败则记录系统日志
        *bszp = bsz;                                                 // 更新缓冲区大小
        *bpp  = bp;                                                  // 更新指向缓冲区的指针
    }
    // 尝试从套接字读取数据到缓冲区
    if ((nr = tread(sockfd, &bp[off], bsz - off, 1)) > 0)
        return (off + nr);  // 读取成功,返回数据结束位置的偏移量
    else
        return (-1);  // 读取失败,返回-1
}

/*
 * 读取并解析打印机响应.如果请求成功,返回1;否则返回0.
 *
 * 锁定:无.
 */
int printer_status(int sfd, struct job *jp)
{
    int             i, success, code, len, found, bufsz, datsz;  // 定义变量用于存储状态码,长度等信息
    int32_t         jobid;                                       // 定义变量用于存储作业ID
    ssize_t         nr;                                          // 定义变量用于存储读取的字节数
    char           *bp, *cp, *statcode, *reason, *contentlen;    // 定义指针用于操作缓冲区
    struct ipp_hdr *hp;                                          // 定义IPP头部结构体指针

    // 分配读取缓冲区
    success = 0;
    bufsz   = IOBUFSZ;
    if ((bp = malloc(IOBUFSZ)) == NULL)
        log_sys("printer_status: can't allocate read buffer");

    // 循环读取HTTP头和IPP响应头
    while ((nr = tread(sfd, bp, bufsz, 5)) > 0)
    {
        // 查找状态码
        // 从 bp 指针偏移 8 个字节的位置开始,将指针赋值给 cp
        cp = bp + 8;

        // 将 nr 的值赋给 datsz,nr 可能表示数据的大小
        datsz = nr;

        // 循环检查 cp 指向的字符是否为空白字符(空格,制表符等)
        // 如果是空白字符,则将 cp 指针向后移动一位,直到遇到非空白字符为止
        while (isspace((int)*cp))
            cp++;

        // 将cp的值赋给statcode,用于后续的状态判断
        statcode = cp;
        // 循环判断cp指向的字符是否为数字
        while (isdigit((int)*cp))
            // 如果是数字,则cp指针向后移动一位
            cp++;

        if (cp == statcode)
        {  // 格式错误;记录并继续
            log_msg(bp);
        }
        else
        {
            // 将当前指针指向的字符设置为字符串结束符
            *cp++ = '\0';
            // 将错误原因字符串的起始地址赋给reason
            reason = cp;
            // 移动指针直到遇到回车符('\r')或换行符('\n')
            while (*cp != '\r' && *cp != '\n')
                cp++;
            // 将遇到的回车符或换行符设置为字符串结束符
            *cp = '\0';
            // 将statcode字符串转换为整数并赋给code
            code = atoi(statcode);
            // 如果HTTP状态码表示信息性消息,则跳过当前循环
            if (HTTP_INFO(code))
                continue;
            // 如果HTTP状态码不表示成功,则记录错误
            if (!HTTP_SUCCESS(code))
            {
                // 将数据缓冲区bp的末尾设置为字符串结束符
                bp[datsz] = '\0';
                // 记录错误信息
                log_msg("error: %s", reason);
                // 跳出循环
                break;
            }

            // HTTP请求正常,但还需要检查IPP状态.搜索Content-Length.
            i = cp - bp;
            for (;;)
            {
                // 当前指针cp指向的字符不是'C'或'c',并且索引i小于数据大小datsz时,继续循环
                while (*cp != 'C' && *cp != 'c' && i < datsz)
                {
                    cp++;  // 移动指针到下一个字符
                    i++;   // 增加索引
                }

                // 如果索引i已经达到或超过数据大小datsz
                if (i >= datsz)
                {
                    // 尝试获取更多的头部信息
                    if ((nr = readmore(sfd, &bp, i, &bufsz)) < 0)
                    {
                        goto out;  // 如果获取失败,跳转到out标签处
                    }
                    else
                    {
                        cp = &bp[i];  // 更新指针cp的位置
                        datsz += nr;  // 增加数据大小datsz
                    }
                }

                // 检查字符串cp是否以"Content-Length:"开头,忽略大小写
                if (strncasecmp(cp, "Content-Length:", 15) == 0)
                {
                    // 如果是,跳过"Content-Length:"这15个字符
                    cp += 15;
                    // 跳过开头的空白字符
                    while (isspace((int)*cp))
                        cp++;
                    // 记录"Content-Length:"后面的字符串,即长度值的开始位置
                    contentlen = cp;
                    // 找到长度值的结束位置,即第一个非数字字符的位置
                    while (isdigit((int)*cp))
                        cp++;
                    // 将长度值的结束位置设置为字符串结束符
                    *cp++ = '\0';
                    // 计算长度值的字符个数
                    i = cp - bp;
                    // 将长度值字符串转换为整数
                    len = atoi(contentlen);
                    // 跳出循环
                    break;
                }
                else
                {
                    // 如果不是以"Content-Length:"开头,继续检查下一个字符
                    cp++;
                    i++;
                }
            }
            // 检查当前数据大小是否满足需求
            if (i >= datsz)
            {
                // 如果不满足,尝试读取更多头部信息
                // readmore函数尝试从sfd读取更多数据到bp,更新i和bufsz
                // 参数:sfd - 文件描述符;&bp - 数据缓冲区指针;i - 当前数据索引;&bufsz - 缓冲区大小
                // 返回值:成功读取的字节数,失败返回-1
                if ((nr = readmore(sfd, &bp, i, &bufsz)) < 0)
                {
                    // 读取失败,跳转到out标签处进行处理
                    goto out;
                }
                else
                {
                    // 读取成功,更新数据指针cp到新的数据位置,更新总数据大小datsz
                    cp = &bp[i];
                    datsz += nr;
                }
            }

            // 初始化找到标志为0
            found = 0;
            // 循环直到找到HTTP头部的结束标志
            while (!found)
            {
                // 遍历数据直到倒数第三个字节
                while (i < datsz - 2)
                {
                    // 检查当前字节和后两个字节是否为HTTP头部的结束标志(\n\r\n)
                    if (*cp == '\n' && *(cp + 1) == '\r' &&
                        *(cp + 2) == '\n')
                    {
                        // 找到结束标志,设置找到标志为1
                        found = 1;
                        // 移动指针和索引跳过结束标志
                        cp += 3;
                        i += 3;
                        break;  // 跳出内层循环
                    }
                    // 移动指针和索引继续查找
                    cp++;
                    i++;
                }
                // 如果索引已经到达数据末尾
                if (i >= datsz)
                {
                    // 尝试获取更多的头部信息
                    if ((nr = readmore(sfd, &bp, i, &bufsz)) < 0)
                    {
                        // 如果读取失败,跳转到错误处理标签
                        goto out;
                    }
                    else
                    {
                        // 更新指针位置和数据大小
                        cp = &bp[i];
                        datsz += nr;
                    }
                }
            }

            // 如果剩余数据大小小于要读取的长度,需要获取更多数据
            if (datsz - i < len)
            {
                // 尝试从文件描述符sfd读取更多数据到缓冲区bp,从位置i开始,最多读取bufsz大小的数据
                // readmore函数的返回值nr表示实际读取的字节数,如果小于0表示读取失败
                if ((nr = readmore(sfd, &bp, i, &bufsz)) < 0)
                {
                    // 读取失败,跳转到out标签处进行错误处理
                    goto out;
                }
                else
                {
                    // 读取成功,更新cp指针指向新读取的数据的起始位置,更新datsz为新的数据总大小
                    cp = &bp[i];
                    datsz += nr;
                }
            }

            // 定义一个结构体指针hp,指向cp指向的内存地址,该内存地址存储了一个ipp_hdr结构体
            hp = (struct ipp_hdr *)cp;

            // 将hp指向的结构体中的status字段从网络字节序转换为主机字节序,并赋值给i
            i = ntohs(hp->status);

            // 将hp指向的结构体中的request_id字段从网络字节序转换为主机字节序,并赋值给jobid
            jobid = ntohl(hp->request_id);

            // 检查当前的jobid是否与jp指向的结构体中的jobid相同
            if (jobid != jp->jobid)
            {
                // 如果不相同,说明是不同的作业,忽略此次操作
                // 记录日志,显示当前的jobid和状态码
                log_msg("jobid %d status code %d", jobid, i);
                break;  // 跳出当前循环
            }

            // 如果状态码表示操作成功,则将success变量设置为1
            if (STATCLASS_OK(i))
                success = 1;
            break;  // 跳出当前循环
        }
    }

out:
    // 释放之前分配的内存
    free(bp);

    // 如果读取打印机响应时发生错误(nr小于0)
    if (nr < 0)
    {
        // 记录错误信息,包括作业ID和系统错误描述
        // 参数1:格式化字符串,包含占位符%d和%s
        // 参数2:作业ID
        // 参数3:通过strerror函数获取的errno对应的错误描述
        log_msg("jobid %d: error reading printer response: %s", jobid, strerror(errno));
    }

    // 返回操作成功与否的状态
    return (success);
}
