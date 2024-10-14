#include <sys/stat.h>  // 包含文件状态信息的结构体和函数声明
#include <dirent.h>    // 包含目录项信息的结构体和函数声明
#include <limits.h>    // 包含各种限制值,如文件名最大长度
#include <string.h>    // 包含字符串操作的函数声明
#include <termios.h>   // 包含终端I/O控制的函数声明
#include <unistd.h>    // 包含系统调用的函数声明,如read(), write()
#include <stdlib.h>    // 包含标准库函数的声明,如malloc(), free()

// 注意:以上头文件的注释是根据其通常用途添加的,具体代码中使用的功能可能需要更详细的注释.

// 定义设备目录结构体
struct devdir
{
    struct devdir *d_next;  // 指向下一个设备目录的指针
    char          *d_name;  // 设备目录的名称
};

// 定义静态变量,用于维护设备目录链表的头尾指针
// 定义静态设备目录结构体指针 head 和 tail
// head 指向设备目录链表的头部
// tail 指向设备目录链表的尾部
static struct devdir *head;
static struct devdir *tail;

// 定义静态字符数组,用于存储路径名
static char pathname[_POSIX_PATH_MAX + 1];

// 添加注释的示例
// 函数功能:将指定目录名添加到系统中
// 参数:
//   dirname - 要添加的目录名,类型为 char 指针
static void
add(char *dirname)
{
    // 定义结构体指针ddp和整型变量len
    // 定义一个指向devdir结构体的指针ddp,用于存储设备目录信息
    struct devdir *ddp;

    // 定义一个整型变量len,用于存储设备目录的长度
    int len;

    // 获取dirname的长度并赋值给len
    len = strlen(dirname);

    /*
     * 跳过特殊目录项:当前目录(.), 上级目录(..)以及/dev/fd
     */
    // 如果dirname以'.'结尾,并且倒数第二个字符是'/',或者是'.'和'/'的组合,则直接返回,不进行后续处理
    if ((dirname[len - 1] == '.') && (dirname[len - 2] == '/' ||
                                      (dirname[len - 2] == '.' && dirname[len - 3] == '/')))
        return;

    // 如果dirname等于"/dev/fd",则直接返回,不进行后续操作
    if (strcmp(dirname, "/dev/fd") == 0)
        return;

    // 分配内存空间用于存储devdir结构体,如果分配失败则返回
    if ((ddp = malloc(sizeof(struct devdir))) == NULL)
        return;

    // 复制dirname字符串到ddp->d_name,如果复制失败则释放之前分配的内存并返回
    if ((ddp->d_name = strdup(dirname)) == NULL)
    {
        // 释放ddp指向的内存空间,防止内存泄漏
        // 由于ddp是一个指针,使用free函数来释放它所指向的内存是必要的
        // 在返回之前释放内存是一个良好的编程习惯,可以避免悬挂指针和内存泄漏问题
        free(ddp);
        return;
    }

    // 初始化ddp指针的下一个节点为NULL
    ddp->d_next = NULL;

    // 如果链表为空(尾节点为NULL),则将头节点和尾节点都设置为ddp
    if (tail == NULL)
    {
        // 初始化链表的头节点和尾节点为同一个节点ddp
        // 这样做通常是为了表示一个空链表或者链表的初始状态
        head = ddp;
        tail = ddp;
    }
    // 如果链表不为空,则将当前尾节点的下一个节点设置为ddp,并更新尾节点为ddp
    else
    {
        // 将ddp节点的下一个指针指向tail的当前下一个节点
        // 这实际上是将ddp链接到链表的尾部
        tail->d_next = ddp;

        // 更新tail指针,使其指向新的链表尾部节点ddp
        tail = ddp;
    }
}

// cleanup 函数用于清理链表中的所有节点,并释放它们占用的内存.
// 它遍历链表,释放每个节点的 d_name 字符串和节点本身的内存,
// 最后将头指针和尾指针设置为 NULL.
static void
cleanup(void)
{
    struct devdir *ddp, *nddp;  // ddp 是当前节点指针,nddp 是下一个节点指针

    ddp = head;          // 从头节点开始
    while (ddp != NULL)  // 遍历链表直到当前节点为空
    {
        nddp = ddp->d_next;  // 保存下一个节点的指针
        free(ddp->d_name);   // 释放当前节点的 d_name 字符串占用的内存
        free(ddp);           // 释放当前节点占用的内存
        ddp = nddp;          // 移动到下一个节点
    }
    head = NULL;  // 清空头指针
    tail = NULL;  // 清空尾指针
}

/*
 * 功能:在指定目录中搜索文件,并返回找到的文件的路径
 * 参数:
 *   dirname - 要搜索的目录名
 *   fdstatp - 用于存储找到的文件的状态信息的结构体指针
 * 返回值:
 *   成功找到文件时返回文件的完整路径,未找到或出错时返回NULL
 */
static char *
searchdir(char *dirname, struct stat *fdstatp)
{
    struct stat    devstat;  // 用于存储目录项的stat信息
    DIR           *dp;       // 目录流指针
    int            devlen;   // pathname的长度
    struct dirent *dirp;     // 目录项结构体指针

    strcpy(pathname, dirname);            // 将dirname复制到pathname
    if ((dp = opendir(dirname)) == NULL)  // 打开目录,失败则返回NULL
        return (NULL);
    strcat(pathname, "/");      // 在pathname后添加斜杠
    devlen = strlen(pathname);  // 获取pathname的长度

    // 遍历目录中的所有项
    // 读取目录中的下一个条目
    // dirp: 指向dirent结构体的指针,用于存储读取到的目录条目信息
    // dp: 指向DIR结构体的指针,表示当前正在读取的目录流
    // 返回值: 成功时返回指向dirent结构体的指针,失败或到达目录末尾时返回NULL
    while ((dirp = readdir(dp)) != NULL)
    {
        strncpy(pathname + devlen, dirp->d_name, _POSIX_PATH_MAX - devlen);  // 将目录项名称复制到pathname

        // 跳过别名
        // 检查路径名是否为标准输入,输出或错误流,如果是,则跳过当前循环
        if (strcmp(pathname, "/dev/stdin") == 0 ||
            strcmp(pathname, "/dev/stdout") == 0 ||
            strcmp(pathname, "/dev/stderr") == 0)
            continue;

        // 获取pathname的stat信息,如果失败则跳过当前循环
        if (stat(pathname, &devstat) < 0)
            continue;

        // 如果pathname是一个目录,则将其添加到路径中并继续下一次循环
        // 检查当前文件状态是否表示一个目录
        if (S_ISDIR(devstat.st_mode))
        {
            // 如果是目录,则调用add函数处理该目录
            add(pathname);
            // 继续下一次循环
            continue;
        }

        // 如果找到匹配的inode和设备号,说明找到了对应的文件,返回pathname
        // 检查当前目录项的设备号和节点号是否与文件描述符的状态匹配
        if (devstat.st_ino == fdstatp->st_ino &&
            devstat.st_dev == fdstatp->st_dev)
        {
            closedir(dp);       // 如果匹配,关闭目录流
            return (pathname);  // 并返回路径名
        }
    }

    closedir(dp);   // 遍历结束,关闭目录流
    return (NULL);  // 未找到匹配项,返回NULL
}

char *
ttyname(int fd)
{
    // 定义文件状态结构体
    struct stat fdstat;
    // 定义设备目录结构体指针
    struct devdir *ddp;
    // 定义返回值指针
    char *rval;

    // 检查文件描述符是否指向终端设备
    if (isatty(fd) == 0)
        return (NULL);
    // 获取文件状态
    if (fstat(fd, &fdstat) < 0)
        return (NULL);
    // 检查文件是否为字符设备
    if (S_ISCHR(fdstat.st_mode) == 0)
        return (NULL);

    // 在/dev目录下搜索设备名称
    rval = searchdir("/dev", &fdstat);
    if (rval == NULL)
    {
        // 遍历设备目录链表,搜索匹配的设备名称
        // 遍历链表,搜索目录
        // 参数:
        //   head: 链表的头节点
        //   searchdir: 用于搜索目录的函数,返回找到的目录项或NULL
        //   fdstat: 用于存储目录项信息的结构体指针
        // 返回:
        //   如果找到目录项,返回对应的ddp节点;否则返回NULL
        for (ddp = head; ddp != NULL; ddp = ddp->d_next)
        {
            // 调用searchdir函数搜索当前节点的目录名
            if ((rval = searchdir(ddp->d_name, &fdstat)) != NULL)
            {
                // 如果找到目录项,跳出循环
                break;
            }
        }
    }

    // 清理资源
    cleanup();
    // 返回设备名称
    return (rval);
}
