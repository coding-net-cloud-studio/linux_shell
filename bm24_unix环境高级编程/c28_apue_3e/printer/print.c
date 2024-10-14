/*
 * 打印文件的客户端命令.打开文件并将其发送到打印机假脱机守护进程.
 * 使用方法:
 *  print [-t] filename
 */

#include "apue.h"   // 引入APUE库头文件
#include "print.h"  // 引入打印功能相关的头文件
#include <fcntl.h>  // 文件控制操作头文件
#include <pwd.h>    // 用户信息头文件

/*
 * 需要用于日志记录的功能.
 */
int log_to_stderr = 1;  // 日志输出到标准错误的标志

/*
 * submit_file - 将文件提交到打印机假脱机系统
 * @fd: 文件描述符
 * @flags: 打开文件时的标志
 * @name: 文件名
 * @len: 文件长度
 * @file_type: 文件类型
 */
void submit_file(int fd, int flags, const char *name, size_t len, int file_type);
int  main(int argc, char *argv[])
{
    int              fd, sfd, err, text, c;  // 定义文件描述符,套接字描述符,错误码,文本标志和字符变量
    struct stat      sbuf;                   // 定义文件状态结构体
    char            *host;                   // 定义主机名指针
    struct addrinfo *ailist, *aip;           // 定义地址信息列表指针和当前地址信息指针

    err  = 0;
    text = 0;
    // 解析命令行参数
    while ((c = getopt(argc, argv, "t")) != -1)
    {
        switch (c)
        {
        case 't':  // 如果指定了-t选项,设置文本标志为1
            text = 1;
            break;
        case '?':  // 如果遇到未知选项,设置错误码为1
            err = 1;
            break;
        }
    }
    // 如果存在错误或者参数数量不正确,打印使用方法并退出
    if (err || (optind != argc - 1))
        err_quit("usage: print [-t] filename");
    // 打开文件,如果失败则打印错误信息并退出
    if ((fd = open(argv[optind], O_RDONLY)) < 0)
        err_sys("print: can't open %s", argv[optind]);
    // 获取文件状态,如果失败则打印错误信息并退出
    if (fstat(fd, &sbuf) < 0)
        err_sys("print: can't stat %s", argv[optind]);
    // 检查是否为普通文件,如果不是则打印错误信息并退出
    if (!S_ISREG(sbuf.st_mode))
        err_quit("print: %s must be a regular file", argv[optind]);

    /*
     * 获取作为打印服务器的主机名.
     */
    // 获取打印服务器的主机名,如果失败则打印错误信息并退出
    if ((host = get_printserver()) == NULL)
        err_quit("print: no print server defined");
    // 获取地址信息列表,如果失败则打印错误信息并退出
    // 如果 err 不等于 0,表示 getaddrlist 函数调用失败
    // getaddrlist 函数尝试解析主机名 host 和服务名 "print",并将结果存储在 ailist 中
    // 函数原型: int getaddrlist(const char *host, const char *service, struct addrinfo **result);
    // 参数:
    //   host - 要解析的主机名
    //   service - 要解析的服务名
    //   result - 指向 addrinfo 结构体指针的指针,用于存储解析结果
    // 返回值:
    //   成功时返回 0,失败时返回非 0 错误码
    if ((err = getaddrlist(host, "print", &ailist)) != 0)

        err_quit("print: getaddrinfo error: %s", gai_strerror(err));

    // 遍历地址信息列表,尝试连接打印服务器
    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        // 尝试连接打印服务器,如果失败则记录错误码
        // 连接服务器,如果连接失败则重试
        // 参数:
        //   AF_INET: 使用IPv4地址族
        //   SOCK_STREAM: 使用TCP流套接字
        //   0: 使用默认协议
        //   aip->ai_addr: 服务器地址信息
        //   aio->ai_addrlen: 地址信息长度
        // 返回值:
        //   成功返回套接字描述符,失败返回-1
        if ((sfd = connect_retry(AF_INET, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0)
        {
            err = errno;
        }
        else
        {
            // 如果连接成功,提交文件进行打印,然后退出程序
            // submit_file函数用于提交文件,参数包括:
            // fd: 目标文件描述符
            // sfd: 源文件描述符
            // argv[optind]: 文件名
            // sbuf.st_size: 文件大小
            // text: 文件内容
            submit_file(fd, sfd, argv[optind], sbuf.st_size, text);

            exit(0);
        }
    }
    // 如果所有地址都尝试失败,打印错误信息并退出
    err_exit(err, "print: can't contact %s", host);
}

/*
 * 发送文件到打印机守护进程.
 */
// submit_file 函数用于将文件提交到服务器.
// 参数:
//   fd: 文件描述符,表示要提交的文件.
//   sockfd: 套接字文件描述符,用于与服务器通信.
//   fname: 要提交的文件名.
//   nbytes: 文件的字节大小.
//   text: 如果文件是文本文件,则为1,否则为0.
void submit_file(int fd, int sockfd, const char *fname, size_t nbytes, int text)
{
    int              nr, nw, len;
    struct passwd   *pwd;           // 用户信息结构体指针
    struct printreq  req;           // 打印请求结构体
    struct printresp res;           // 打印响应结构体
    char             buf[IOBUFSZ];  // 缓冲区,用于存储从文件读取的数据

    /*
     * 首先构建请求头.
     */
    if ((pwd = getpwuid(geteuid())) == NULL)  // 获取当前用户的密码信息
    {
        strcpy(req.usernm, "unknown");  // 如果获取失败,用户名设为"unknown"
    }
    else
    {
        strncpy(req.usernm, pwd->pw_name, USERNM_MAX - 1);  // 复制用户名到请求结构体
        req.usernm[USERNM_MAX - 1] = '\0';                  // 确保字符串以空字符结尾
    }
    req.size = htonl(nbytes);  // 将文件大小转换为网络字节序并存储

    if (text)
        req.flags = htonl(PR_TEXT);  // 如果是文本文件,设置相应的标志
    else
        req.flags = 0;  // 否则标志为0

    if ((len = strlen(fname)) >= JOBNM_MAX)  // 如果文件名过长
    {
        /*
         * 截断文件名(+-5是为了加上前面的三个点,空格以及结尾的空字符).
         */
        strcpy(req.jobnm, "... ");
        strncat(req.jobnm, &fname[len - JOBNM_MAX + 5], JOBNM_MAX - 5);  // 截断并复制文件名
    }
    else
    {
        strcpy(req.jobnm, fname);  // 否则直接复制文件名
    }

    /*
     * 发送请求头到服务器.
     */
    nw = writen(sockfd, &req, sizeof(struct printreq));  // 写入请求头
    if (nw != sizeof(struct printreq))                   // 检查写入的字节数
    {
        if (nw < 0)
            err_sys("can't write to print server");  // 写入失败
        else
            err_quit("short write (%d/%d) to print server",  // 写入字节数不足
                     nw,
                     sizeof(struct printreq));
    }

    /*
     * 现在发送文件内容.
     */
    while ((nr = read(fd, buf, IOBUFSZ)) != 0)  // 从文件读取数据到缓冲区
    {
        nw = writen(sockfd, buf, nr);  // 将缓冲区的数据写入到服务器
        if (nw != nr)                  // 检查写入的字节数
        {
            if (nw < 0)
                err_sys("can't write to print server");  // 写入失败
            else
                err_quit("short write (%d/%d) to print server",  // 写入字节数不足
                         nw,
                         nr);
        }
    }

    /*
     * 读取服务器的响应.
     */
    if ((nr = readn(sockfd, &res, sizeof(struct printresp))) !=  // 读取响应
        sizeof(struct printresp))
        err_sys("can't read response from server");  // 读取失败
    if (res.retcode != 0)                            // 如果响应码不为0,表示请求被拒绝
    {
        printf("rejected: %s\n", res.msg);  // 打印拒绝信息
        exit(1);                            // 退出程序
    }
    else
    {
        printf("job ID %ld\n", (long)ntohl(res.jobid));  // 打印作业ID
    }
}
