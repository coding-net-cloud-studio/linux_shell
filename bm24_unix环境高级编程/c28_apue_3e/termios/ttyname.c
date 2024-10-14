#include <sys/stat.h>  // 包含文件状态信息结构体和相关函数声明
#include <dirent.h>    // 包含目录项信息的结构体和相关函数声明
#include <limits.h>    // 包含定义各种限制的宏
#include <string.h>    // 包含字符串操作的函数声明
#include <termios.h>   // 包含终端I/O控制的结构体和函数声明
#include <unistd.h>    // 包含Unix系统服务的函数声明
#include <stdlib.h>    // 包含通用工具函数声明

// 定义设备目录结构体
struct devdir
{
    struct devdir *d_next;  // 指向下一个设备目录项的指针
    char          *d_name;  // 设备目录项的名称
};

// 定义静态变量,用于维护设备目录链表的头尾指针
static struct devdir *head;
static struct devdir *tail;

// 定义静态字符数组,用于存储路径名
static char pathname[_POSIX_PATH_MAX + 1];

static void
add(char *dirname)
{
    // 定义结构体指针和长度变量
    struct devdir *ddp;
    int            len;

    // 获取dirname的长度
    len = strlen(dirname);

    // 跳过特殊目录名,如当前目录,上级目录和/dev/fd
    // 检查目录名是否以斜杠和点结尾,如果是,则返回,不进行处理
    // 例如:"/path/to/dir." 或 "/path/to/."
    if ((dirname[len - 1] == '.') && (dirname[len - 2] == '/' ||
                                      (dirname[len - 2] == '.' && dirname[len - 3] == '/')))
        return;

    // 如果目录名是 "/dev/fd",则直接返回,不进行处理
    // "/dev/fd" 通常用于表示文件描述符
    if (strcmp(dirname, "/dev/fd") == 0)
        return;

    // 分配内存空间给结构体指针
    if ((ddp = malloc(sizeof(struct devdir))) == NULL)
        return;

    // 复制dirname到结构体的d_name字段
    if ((ddp->d_name = strdup(dirname)) == NULL)
    {
        free(ddp);  // 如果复制失败,释放已分配的内存
        return;
    }

    // 初始化结构体的d_next字段为NULL
    ddp->d_next = NULL;

    // 如果链表为空,则新节点既是头节点也是尾节点
    if (tail == NULL)
    {
        head = ddp;
        tail = ddp;
    }
    else
    {
        // 否则,将新节点添加到链表尾部
        tail->d_next = ddp;
        tail         = ddp;
    }
}

static void
cleanup(void)  // 清理函数,用于释放设备目录结构体链表占用的内存
{
    struct devdir *ddp, *nddp;  // 定义两个指向devdir结构体的指针,ddp用于遍历链表,nddp用于保存下一个节点

    ddp = head;          // 将ddp指向链表的头节点
    while (ddp != NULL)  // 遍历链表直到ddp为NULL
    {
        nddp = ddp->d_next;  // 将nddp指向ddp的下一个节点
        free(ddp->d_name);   // 释放ddp节点中d_name指向的内存
        free(ddp);           // 释放ddp节点占用的内存
        ddp = nddp;          // 将ddp移动到下一个节点
    }
    head = NULL;  // 清空链表头指针
    tail = NULL;  // 清空链表尾指针
}

static char *
searchdir(char *dirname, struct stat *fdstatp)
{
    struct stat    devstat;  // 用于存储目录项的状态信息
    DIR           *dp;       // 目录流指针
    int            devlen;   // 路径名长度
    struct dirent *dirp;     // 目录项结构指针

    strcpy(pathname, dirname);            // 复制目录名到pathname
    if ((dp = opendir(dirname)) == NULL)  // 打开目录,如果失败返回NULL
        return (NULL);
    strcat(pathname, "/");                // 在pathname后追加斜杠
    devlen = strlen(pathname);            // 获取pathname的长度
    while ((dirp = readdir(dp)) != NULL)  // 读取目录项,直到没有更多项
    {
        strncpy(pathname + devlen, dirp->d_name, _POSIX_PATH_MAX - devlen);  // 将目录项名称复制到pathname

        // 跳过标准输入输出错误流别名
        // 检查pathname是否为标准输入,输出或错误设备,如果是,则跳过当前循环
        // 检查路径名是否为标准输入,标准输出或标准错误
        // 如果是,则跳过当前循环迭代
        if (strcmp(pathname, "/dev/stdin") == 0 ||   // 比较路径名是否为标准输入
            strcmp(pathname, "/dev/stdout") == 0 ||  // 比较路径名是否为标准输出
            strcmp(pathname, "/dev/stderr") == 0)    // 比较路径名是否为标准错误
            continue;                                // 如果是标准输入,输出或错误,跳过本次循环

        // 获取pathname的状态,如果失败(返回值小于0),则跳过当前循环
        // stat函数用于获取文件或目录的状态信息
        if (stat(pathname, &devstat) < 0)
            continue;

        // 检查pathname是否为目录,如果是,则调用add函数将其添加到路径并继续下一次循环
        // S_ISDIR是一个宏,用于检查文件模式是否表示一个目录
        if (S_ISDIR(devstat.st_mode))
        {
            add(pathname);
            continue;
        }

        // 如果找到匹配的设备号和节点号,则关闭目录流并返回pathname
        // 检查当前目录项的设备号和节点号是否与文件描述符的状态匹配
        if (devstat.st_ino == fdstatp->st_ino &&
            devstat.st_dev == fdstatp->st_dev)
        {
            // 如果匹配,则关闭目录指针并返回路径名
            closedir(dp);
            return (pathname);
        }
    }

    closedir(dp);   // 关闭目录流
    return (NULL);  // 没有找到匹配项,返回NULL
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

    // 检查文件描述符是否指向终端,如果不是则返回NULL
    if (isatty(fd) == 0)
        return (NULL);
    // 获取文件描述符的状态,如果失败则返回NULL
    if (fstat(fd, &fdstat) < 0)
        return (NULL);
    // 检查文件状态是否表示字符设备,如果不是则返回NULL
    if (S_ISCHR(fdstat.st_mode) == 0)
        return (NULL);

    // 在/dev目录下搜索与文件描述符状态匹配的设备名称
    rval = searchdir("/dev", &fdstat);
    // 如果没有找到,则遍历设备目录链表继续搜索
    // 如果rval为NULL,则遍历链表
    if (rval == NULL)
    {
        // 从头节点开始遍历
        for (ddp = head; ddp != NULL; ddp = ddp->d_next)
            // 如果在当前目录项中找到了rval,则跳出循环
            if ((rval = searchdir(ddp->d_name, &fdstat)) != NULL)
                break;
    }

    // 清理资源
    cleanup();
    // 返回找到的设备名称或NULL
    return (rval);
}
