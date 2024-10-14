#include "apue.h"                              // 引入APUE库的头文件
#include "apue_db.h"                           // 引入APUE数据库操作的头文件
#include <fcntl.h> /* open & db_open flags */  // 包含文件打开标志的定义
#include <stdarg.h>                            // 包含可变参数列表的定义
#include <errno.h>                             // 包含错误码的定义
#include <sys/uio.h> /* struct iovec */        // 包含用于散布/聚集I/O的结构体定义

/*
 * 内部索引文件常量.
 * 这些用于构建索引文件和数据文件中的记录.
 */
#define IDXLEN_SZ 4     // 索引记录长度(ASCII字符)
#define SEP       ':'   // 索引记录中的分隔符
#define SPACE     ' '   // 空格字符
#define NEWLINE   '\n'  // 换行符

/*
 * 以下定义用于索引文件中的哈希链和空闲链.
 */
#define PTR_SZ    7        // 哈希链中指针字段的大小
#define PTR_MAX   9999999  // 最大文件偏移量 = 10**PTR_SZ - 1
#define NHASH_DEF 137      // 默认哈希表大小
#define FREE_OFF  0        // 索引文件中空闲列表的偏移量
#define HASH_OFF  PTR_SZ   // 索引文件中哈希表的偏移量

typedef unsigned long DBHASH;  // 哈希值
typedef unsigned long COUNT;   // 无符号计数器

/*
 * 数据库的私有表示.
 */
typedef struct
{
    int   idxfd;          // 索引文件的文件描述符
    int   datfd;          // 数据文件的文件描述符
    char *idxbuf;         // 为索引记录分配的缓冲区
    char *datbuf;         // 为数据记录分配的缓冲区
    char *name;           // 打开数据库时使用的名称
    off_t idxoff;         // 索引文件中索引记录的偏移量
                          // 键位于 (idxoff + PTR_SZ + IDXLEN_SZ)
    size_t idxlen;        // 索引记录的长度
                          // 不包括记录前面的IDXLEN_SZ字节
                          // 包括记录末尾的换行符
    off_t  datoff;        // 数据文件中数据记录的偏移量
    size_t datlen;        // 数据记录的长度
                          // 包括末尾的换行符
    off_t  ptrval;        // 索引记录中链表指针的内容
    off_t  ptroff;        // 指向此索引记录的链表指针偏移量
    off_t  chainoff;      // 此索引记录的哈希链偏移量
    off_t  hashoff;       // 索引文件中哈希表的偏移量
    DBHASH nhash;         // 当前哈希表的大小
    COUNT  cnt_delok;     // 删除成功
    COUNT  cnt_delerr;    // 删除错误
    COUNT  cnt_fetchok;   // 获取成功
    COUNT  cnt_fetcherr;  // 获取错误
    COUNT  cnt_nextrec;   // 下一条记录
    COUNT  cnt_stor1;     // 存储: DB_INSERT, 没有空位, 追加
    COUNT  cnt_stor2;     // 存储: DB_INSERT, 找到空位, 重用
    COUNT  cnt_stor3;     // 存储: DB_REPLACE, 长度不同, 追加
    COUNT  cnt_stor4;     // 存储: DB_REPLACE, 长度相同, 覆盖
    COUNT  cnt_storerr;   // 存储错误
} DB;

/*
 * 内部函数.
 */

// 分配一个新的数据库结构体实例
static DB *_db_alloc(int);

// 删除指定的数据库条目
static void _db_dodelete(DB *);

// 查找并锁定数据库中的指定键,返回状态码
static int _db_find_and_lock(DB *, const char *, int);

// 在数据库中查找空闲位置,用于存储新的键值对
static int _db_findfree(DB *, int, int);

// 释放数据库结构体占用的内存
static void _db_free(DB *);

// 计算给定键的哈希值
static DBHASH _db_hash(DB *, const char *);

// 读取数据库条目的数据部分
static char *_db_readdat(DB *);

// 从数据库索引文件中读取索引信息
static off_t _db_readidx(DB *, off_t);

// 从数据库指针文件中读取指针信息
static off_t _db_readptr(DB *, off_t);

// 将数据写入数据库条目
static void _db_writedat(DB *, const char *, off_t, int);

// 将索引信息写入数据库索引文件
static void _db_writeidx(DB *, const char *, off_t, int, off_t);

// 将指针信息写入数据库指针文件
static void _db_writeptr(DB *, off_t, off_t);

/*
 * 打开或创建一个数据库.参数同 open(2).
 */
DBHANDLE
db_open(const char *pathname, int oflag, ...)
{
    DB    *db;         // 数据库结构体指针
    int    len, mode;  // 文件路径长度和创建模式
    size_t i;
    char   asciiptr[PTR_SZ + 1],             // 指针大小字符串
        hash[(NHASH_DEF + 1) * PTR_SZ + 2];  // 哈希表字符串
    struct stat statbuff;                    // 文件状态结构体

    /*
     * 分配一个 DB 结构体,以及它需要的缓冲区.
     */
    len = strlen(pathname);
    if ((db = _db_alloc(len)) == NULL)  // 分配 DB 结构体
        err_dump("db_open: _db_alloc error for DB");

    db->nhash   = NHASH_DEF;     // 哈希表大小
    db->hashoff = HASH_OFF;      // 索引文件中哈希表的偏移量
    strcpy(db->name, pathname);  // 复制路径名到 DB 结构体
    strcat(db->name, ".idx");    // 添加索引文件扩展名

    if (oflag & O_CREAT)  // 如果指定了创建标志
    {
        va_list ap;
        va_start(ap, oflag);     // 初始化参数列表
        mode = va_arg(ap, int);  // 获取创建模式
        va_end(ap);

        /*
         * 打开索引文件和数据文件.
         */
        db->idxfd = open(db->name, oflag, mode);  // 打开或创建索引文件
        strcpy(db->name + len, ".dat");           // 修改文件名为数据文件名
        db->datfd = open(db->name, oflag, mode);  // 打开或创建数据文件
    }
    else
    {
        /*
         * 打开索引文件和数据文件.
         */
        db->idxfd = open(db->name, oflag);  // 打开索引文件
        strcpy(db->name + len, ".dat");     // 修改文件名为数据文件名
        db->datfd = open(db->name, oflag);  // 打开数据文件
    }

    if (db->idxfd < 0 || db->datfd < 0)  // 检查文件描述符是否有效
    {
        _db_free(db);  // 释放 DB 结构体
        return (NULL);
    }

    if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC))  // 如果数据库是新创建的
    {
        /*
         * 如果数据库是新创建的,我们需要初始化它.
         * 写锁整个文件,以便我们可以对其进行 stat,检查大小并初始化它,这些操作是原子的.
         */

        // 尝试对数据库文件描述符db->idxfd进行写锁,锁定从文件开始位置(SEEK_SET)的0字节偏移量,锁定长度为0(表示整个文件)
        // 如果锁定失败,调用err_dump函数输出错误信息并终止程序
        if (writew_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
            err_dump("db_open: writew_lock error");

        // 获取文件状态信息到statbuff结构体中
        // 如果获取失败,调用err_sys函数输出错误信息并终止程序
        if (fstat(db->idxfd, &statbuff) < 0)
            err_sys("db_open: fstat error");

        if (statbuff.st_size == 0)  // 如果文件大小为0,即文件为空
        {
            /*
             * 我们必须构建一个包含 (NHASH_DEF + 1) 个链表指针的列表,其值为 0.
             * +1 是为了在哈希表之前的空闲列表指针.
             */

            // 使用sprintf函数格式化字符串,将整数0按照PTR_SZ指定的宽度转换为字符串,并存储在asciiptr指向的内存中
            // %*d中的*表示动态宽度,由PTR_SZ指定
            sprintf(asciiptr, "%*d", PTR_SZ, 0);

            // 初始化哈希表的第一个元素为0
            hash[0] = 0;

            // 循环NHASH_DEF + 1次,每次将asciiptr指向的字符串拼接到hash字符串后面
            // 这样做是为了初始化哈希表,确保每个槽位都有相同的初始值
            for (i = 0; i < NHASH_DEF + 1; i++)
                strcat(hash, asciiptr);

            // 在hash字符串末尾添加换行符,以便于后续处理
            strcat(hash, "\n");

            // 获取hash字符串的长度
            i = strlen(hash);

            // 将初始化好的哈希表写入到数据库的索引文件中
            // write函数返回实际写入的字节数,如果不等于hash字符串的长度,说明写入过程中出现了错误
            if (write(db->idxfd, hash, i) != i)
                // 如果写入错误,调用err_dump函数打印错误信息并终止程序
                err_dump("db_open: index file init write error");
        }

        // 尝试解锁数据库文件
        // 参数:
        //   - db->idxfd: 文件描述符
        //   - 0: 锁定的起始位置
        //   - SEEK_SET: 从文件开始位置锁定
        //   - 0: 锁定的长度(0表示整个文件)
        // 返回值:
        //   - 成功返回0,失败返回负数
        if (un_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
            // 如果解锁失败,打印错误信息并退出程序
            err_dump("db_open: un_lock error");
    }
    db_rewind(db);  // 重置数据库读写位置
    return (db);    // 返回数据库句柄
}

/*
 * 分配并初始化一个DB结构体及其缓冲区.
 */
static DB *
_db_alloc(int namelen)
{
    DB *db;  // 定义DB结构体指针

    /*
     * 使用calloc分配内存,并将结构体初始化为零.
     */
    if ((db = calloc(1, sizeof(DB))) == NULL)        // 分配内存
        err_dump("_db_alloc: calloc error for DB");  // 内存分配失败则输出错误信息
    db->idxfd = db->datfd = -1;                      // 初始化文件描述符为-1

    /*
     * 为名称分配空间.
     * 加5是为了".idx"或".dat"以及末尾的空字符.
     */
    if ((db->name = malloc(namelen + 5)) == NULL)      // 分配内存
        err_dump("_db_alloc: malloc error for name");  // 内存分配失败则输出错误信息

    /*
     * 分配索引缓冲区和数据缓冲区.
     * 加2是为了换行符和末尾的空字符.
     */
    if ((db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL)         // 分配内存
        err_dump("_db_alloc: malloc error for index buffer");  // 内存分配失败则输出错误信息
    if ((db->datbuf = malloc(DATLEN_MAX + 2)) == NULL)         // 分配内存
        err_dump("_db_alloc: malloc error for data buffer");   // 内存分配失败则输出错误信息
    return (db);                                               // 返回初始化后的DB结构体指针
}

/*
 * 释放对数据库的访问权限.
 *
 * @param h 数据库句柄,类型为 DBHANDLE
 * @return 无返回值
 *
 * 该函数通过调用_db_free来关闭文件描述符,释放缓冲区和结构体.
 */
void db_close(DBHANDLE h)
{
    _db_free((DB *)h); /* 关闭文件描述符,释放缓冲区和结构体 */
}

/*
 * 释放DB结构及其可能指向的所有malloc分配的缓冲区.如果文件描述符仍然打开,也关闭它们.
 *
 * @param db 指向要释放的DB结构的指针.
 */
static void
_db_free(DB *db)
{
    // 如果idxfd文件描述符有效,则关闭它
    if (db->idxfd >= 0)
        close(db->idxfd);
    // 如果datfd文件描述符有效,则关闭它
    if (db->datfd >= 0)
        close(db->datfd);
    // 如果idxbuf缓冲区被分配,则释放它
    if (db->idxbuf != NULL)
        free(db->idxbuf);
    // 如果datbuf缓冲区被分配,则释放它
    if (db->datbuf != NULL)
        free(db->datbuf);
    // 如果name指针指向的内存被分配,则释放它
    if (db->name != NULL)
        free(db->name);
    // 最后释放DB结构本身
    free(db);
}

/*
 * 从数据库中获取一条记录.返回一个指向以空字符结尾的数据的指针.
 *
 * @param h 数据库句柄
 * @param key 要查找的记录的键
 * @return 如果找到记录,则返回指向数据的指针;否则返回NULL
 */
char *
db_fetch(DBHANDLE h, const char *key)
{
    DB   *db = h;  // 将数据库句柄转换为DB结构体指针
    char *ptr;     // 用于存储找到的记录数据的指针

    // 尝试查找并锁定记录
    if (_db_find_and_lock(db, key, 0) < 0)
    {
        ptr = NULL;          // 如果未找到记录,则设置ptr为NULL
        db->cnt_fetcherr++;  // 增加未找到记录的错误计数
    }
    else
    {
        ptr = _db_readdat(db);  // 如果找到记录,则读取数据并返回指针
        db->cnt_fetchok++;      // 增加成功找到记录的计数
    }

    /*
     * 解锁_db_find_and_lock锁定的哈希链.
     */
    if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
        err_dump("db_fetch: un_lock error");  // 如果解锁失败,则输出错误信息

    return (ptr);  // 返回指向数据的指针或NULL
}

/*
 * 查找指定的记录.被 db_delete,db_fetch 和 db_store 调用.
 * 返回时,哈希链已被锁定.
 */
static int
_db_find_and_lock(DB *db, const char *key, int writelock)
{
    off_t offset, nextoffset;

    /*
     * 计算这个键的哈希值,然后计算哈希表中对应链表指针的字节偏移量.
     * 这是我们的搜索起点.首先我们计算这个键在哈希表中的偏移量.
     */
    db->chainoff = (_db_hash(db, key) * PTR_SZ) + db->hashoff;
    db->ptroff   = db->chainoff;

    /*
     * 在这里锁定哈希链.调用者完成操作后必须解锁.
     * 注意我们只锁定和解锁第一个字节.
     */
    if (writelock)
    {
        // 如果需要写锁,则尝试获取写锁
        // 如果writew_lock函数调用失败,则输出错误信息
        // writew_lock函数尝试获取指定文件的写锁
        // db->idxfd: 文件描述符
        // db->chainoff: 锁定的文件偏移量
        // SEEK_SET: 从文件开始位置计算偏移量
        // 1: 锁定的字节数量
        if (writew_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
            // err_dump函数用于输出错误信息并终止程序
            // "_db_find_and_lock: writew_lock error" 是错误信息的描述
            err_dump("_db_find_and_lock: writew_lock error");
    }
    else
    {
        // 如果需要读锁,则尝试获取读锁
        // _db_find_and_lock: 尝试对数据库文件进行读锁定
        // db: 指向数据库结构的指针
        // db->idxfd: 数据库文件的文件描述符
        // db->chainoff: 锁定的起始偏移量
        // SEEK_SET: 表示从文件开始位置进行锁定
        // 1: 锁定的字节数量
        if (readw_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
            // 如果锁定失败,调用err_dump函数打印错误信息并退出程序
            err_dump("_db_find_and_lock: readw_lock error");
    }

    /*
     * 获取哈希链上第一条记录在索引文件中的偏移量(可能为0).
     */
    offset = _db_readptr(db, db->ptroff);
    while (offset != 0)
    {
        nextoffset = _db_readidx(db, offset);
        // 如果找到匹配的键,则跳出循环
        if (strcmp(db->idxbuf, key) == 0)
            break;
        // 更新当前记录的偏移量,并继续查找下一个记录
        db->ptroff = offset;
        offset     = nextoffset;
    }
    /*
     * 如果 offset == 0 表示发生错误(记录未找到).
     */
    return (offset == 0 ? -1 : 0);
}

/*
 * 计算给定键的哈希值.
 *
 * @param db 指向数据库结构的指针,其中包含哈希表的大小.
 * @param key 要计算哈希值的键的指针.
 * @return 返回计算出的哈希值.
 */
static DBHASH
_db_hash(DB *db, const char *key)
{
    DBHASH hval = 0;  // 初始化哈希值为0
    char   c;         // 用于存储当前遍历到的字符
    int    i;         // 用于存储字符的索引,从1开始

    // 遍历键中的每个字符,直到遇到字符串结束符'\0'
    for (i = 1; (c = *key++) != 0; i++)
        hval += c * i;  // 将字符的ASCII值乘以其基于1的索引,并累加到哈希值中

    return (hval % db->nhash);  // 返回哈希值对哈希表大小取模的结果,以确保哈希值在有效范围内
}

/*
 * 从索引文件的任意位置读取链表指针字段:
 * 空闲列表指针,哈希表链表指针或索引记录链表指针.
 *
 * @param db 数据库结构体指针
 * @param offset 要读取的指针字段的偏移量
 * @return 返回读取到的指针值
 */
static off_t
_db_readptr(DB *db, off_t offset)
{
    char asciiptr[PTR_SZ + 1];  // 定义一个字符数组来存储读取到的指针字段的ASCII表示

    // 使用lseek函数将文件指针移动到指定的偏移量
    if (lseek(db->idxfd, offset, SEEK_SET) == -1)
        err_dump("_db_readptr: lseek error to ptr field");  // 如果lseek失败,打印错误信息并退出

    // 从当前文件位置读取PTR_SZ个字节到asciiptr数组
    if (read(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
        err_dump("_db_readptr: read error of ptr field");  // 如果read失败,打印错误信息并退出

    asciiptr[PTR_SZ] = 0;     // 确保asciiptr数组以空字符结尾,以便可以将其作为C字符串处理
    return (atol(asciiptr));  // 将asciiptr转换为长整型数并返回
}

/*
 * 读取下一个索引记录.我们从索引文件的指定偏移量开始.
 * 我们将索引记录读入db->idxbuf,并用空字节替换分隔符.
 * 如果一切正常,我们将设置db->datoff和db->datlen为数据文件中相应数据记录的偏移量和长度.
 */
static off_t
_db_readidx(DB *db, off_t offset)
{
    ssize_t      i;                                              // 用于存储readv和read系统调用的返回值
    char        *ptr1, *ptr2;                                    // 用于定位索引记录中的分隔符
    char         asciiptr[PTR_SZ + 1], asciilen[IDXLEN_SZ + 1];  // 用于存储索引记录前部的ascii链指针和ascii长度
    struct iovec iov[2];                                         // 用于readv系统调用的结构体数组

    /*
     * 定位索引文件并记录偏移量.db_nextrec调用我们时,如果offset==0,意味着从当前偏移量读取.
     * 我们仍然需要调用lseek来记录当前偏移量.
     */
    if ((db->idxoff = lseek(db->idxfd, offset, offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
        err_dump("_db_readidx: lseek error");

    /*
     * 读取索引记录前部的ascii链指针和ascii长度.
     * 这告诉我们索引记录的剩余大小.
     */
    // 从文件 db.c 中选取的代码段,用于从数据库文件中读取索引记录.
    // readv 函数尝试从文件描述符 db->idxfd 读取两个 iovec 结构体指向的数据.
    // 如果读取的字节数不等于 PTR_SZ + IDXLEN_SZ,则表示读取错误或 EOF.

    // 定义 iovec 结构体数组,用于指定要读取的内存区域和长度
    iov[0].iov_base = asciiptr;   // 第一个内存区域的起始地址
    iov[0].iov_len  = PTR_SZ;     // 第一个内存区域的长度
    iov[1].iov_base = asciilen;   // 第二个内存区域的起始地址
    iov[1].iov_len  = IDXLEN_SZ;  // 第二个内存区域的长度

    // 使用 readv 函数尝试读取数据
    if ((i = readv(db->idxfd, &iov[0], 2)) != PTR_SZ + IDXLEN_SZ)
    {
        // 如果读取的字节数为0且偏移量为0,表示到达文件末尾(EOF)
        if (i == 0 && offset == 0)
            return (-1);  // EOF for db_nextrec

        // 否则,打印错误信息并退出
        err_dump("_db_readidx: readv error of index record");
    }

    /*
     * 这是我们的返回值;总是 >= 0.
     */
    asciiptr[PTR_SZ] = 0;               // 空终止符
    db->ptrval       = atol(asciiptr);  // 下一个键在链中的偏移量

    // 为长度数组asciilen赋值空终止符
    asciilen[IDXLEN_SZ] = 0;  // 空终止符

    // 将asciilen转换为整数并赋值给db->idxlen
    // 检查idxlen是否在有效范围内,如果不在,则调用err_dump函数报告错误
    if ((db->idxlen = atoi(asciilen)) < IDXLEN_MIN ||
        db->idxlen > IDXLEN_MAX)
        err_dump("_db_readidx: invalid length");

    /*
     * 现在读取实际的索引记录.我们将其读入我们在打开数据库时malloc的键缓冲区.
     */
    if ((i = read(db->idxfd, db->idxbuf, db->idxlen)) != db->idxlen)
        err_dump("_db_readidx: read error of index record");
    if (db->idxbuf[db->idxlen - 1] != NEWLINE)  // 完整性检查
        err_dump("_db_readidx: missing newline");
    db->idxbuf[db->idxlen - 1] = 0;  // 用空替换换行符

    /*
     * 在索引记录中找到分隔符.
     */
    // 查找第一个分隔符SEP,并用空字符替换它
    if ((ptr1 = strchr(db->idxbuf, SEP)) == NULL)
        err_dump("_db_readidx: missing first separator");
    *ptr1++ = 0;  // 用空替换SEP

    // 查找第二个分隔符SEP,并用空字符替换它
    if ((ptr2 = strchr(ptr1, SEP)) == NULL)
        err_dump("_db_readidx: missing second separator");
    *ptr2++ = 0;  // 用空替换SEP

    // 确保没有更多的分隔符SEP
    if (strchr(ptr2, SEP) != NULL)
        err_dump("_db_readidx: too many separators");

    /*
     * 获取数据记录的起始偏移量和长度.
     */
    if ((db->datoff = atol(ptr1)) < 0)  // 将字符串 ptr1 转换为长整型并赋值给 db->datoff,如果转换后的值小于0,则调用 err_dump 函数报告错误
        err_dump("_db_readidx: starting offset < 0");
    if ((db->datlen = atol(ptr2)) <= 0 || db->datlen > DATLEN_MAX)  // 将字符串 ptr2 转换为长整型并赋值给 db->datlen,如果转换后的值小于等于0或者大于 DATLEN_MAX,则调用 err_dump 函数报告错误
        err_dump("_db_readidx: invalid length");
    return (db->ptrval);  // 返回链中下一个键的偏移量
}

/*
 * 从数据库文件中读取当前数据记录到数据缓冲区.
 * 返回指向以空字符结尾的数据缓冲区的指针.
 *
 * @param db 指向数据库结构的指针
 * @return 指向数据记录的指针
 */
static char *
_db_readdat(DB *db)
{
    // 将文件指针移动到数据记录的开始位置
    if (lseek(db->datfd, db->datoff, SEEK_SET) == -1)
        err_dump("_db_readdat: lseek error");  // 如果lseek失败,输出错误并退出

    // 从文件描述符db->datfd读取db->datlen字节到db->datbuf
    if (read(db->datfd, db->datbuf, db->datlen) != db->datlen)
        err_dump("_db_readdat: read error");  // 如果read失败,输出错误并退出

    // 检查数据缓冲区的最后一个字符是否为换行符,这是一个健全性检查
    if (db->datbuf[db->datlen - 1] != NEWLINE)
        err_dump("_db_readdat: missing newline");  // 如果没有找到换行符,输出错误并退出

    // 将数据缓冲区的最后一个字符(换行符)替换为空字符,以便数据可以作为C字符串处理
    db->datbuf[db->datlen - 1] = 0;

    // 返回指向数据记录的指针
    return (db->datbuf);
}

/*
 * 删除指定的记录.
 *
 * @param h 数据库句柄
 * @param key 要删除的记录的键
 * @return 如果记录被成功删除返回0,如果记录未找到返回-1
 */
int db_delete(DBHANDLE h, const char *key)
{
    DB *db = h;  // 将数据库句柄转换为DB结构体指针
    int rc = 0;  // 假设记录将被找到

    // 尝试查找并锁定记录,如果成功返回0
    if (_db_find_and_lock(db, key, 1) == 0)
    {
        _db_dodelete(db);  // 执行删除操作
        db->cnt_delok++;   // 增加成功删除的记录计数
    }
    else
    {
        rc = -1;           // 记录未找到
        db->cnt_delerr++;  // 增加删除错误的记录计数
    }
    // 尝试解锁,如果失败则输出错误信息
    if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
        err_dump("db_delete: un_lock error");
    return (rc);  // 返回删除结果
}

/*
 * 删除由DB结构指定的当前记录.
 * 该函数由db_delete和db_store调用,在记录被_db_find_and_lock定位之后.
 */
static void
_db_dodelete(DB *db)
{

    // 定义整型变量i,用于循环或其他整数操作
    int i;

    // 定义字符指针ptr,可能用于指向字符串或字符数组
    char *ptr;

    // 定义off_t类型的freeptr和saveptr,通常用于文件操作中表示文件指针的位置
    // off_t是POSIX标准中定义的一种类型,用于表示文件偏移量
    off_t freeptr, saveptr;

    /*
     * 将数据缓冲区和键全部设置为空格.
     */
    // 以下代码段用于将数据库缓冲区清空并初始化为空格字符,最后添加null终止符
    // db->datbuf 是数据缓冲区的指针
    // db->datlen 是数据缓冲区的长度
    // SPACE 是一个宏定义,代表空格字符

    // 清空数据缓冲区并添加null终止符
    for (ptr = db->datbuf, i = 0; i < db->datlen - 1; i++)
    {
        *ptr++ = SPACE;  // 将空格字符赋值给缓冲区的每个位置,并移动指针
    }
    *ptr = 0;  // 在缓冲区末尾添加null终止符,以便正确结束字符串

    // 清空索引缓冲区
    ptr = db->idxbuf;
    while (*ptr)
    {
        *ptr++ = SPACE;  // 将空格字符赋值给索引缓冲区的每个位置,并移动指针,直到遇到null终止符
    }

    /*
     * 我们必须锁定空闲列表.
     */
    // _db_dodelete 函数中,尝试对数据库文件进行写锁操作
    // db->idxfd 是数据库文件的文件描述符
    // FREE_OFF 是要锁定的偏移量
    // SEEK_SET 表示从文件开始位置进行锁定
    // 1 表示锁定一个字节的范围
    // 如果锁定失败(返回值小于0),则调用 err_dump 函数输出错误信息
    if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("_db_dodelete: writew_lock error");

    /*
     * 写入全是空格的数据记录.
     */
    _db_writedat(db, db->datbuf, db->datoff, SEEK_SET);

    /*
     * 读取空闲列表指针.它的值成为已删除索引记录的链表指针字段.
     * 这意味着已删除的记录成为空闲列表的头.
     */
    freeptr = _db_readptr(db, FREE_OFF);

    /*
     * 在它被_db_writeidx重写之前,保存索引记录链表指针的内容.
     */
    saveptr = db->ptrval;

    /*
     * 重写索引记录.这也重写了索引记录的长度,数据偏移量和数据长度,
     * 这些都没有改变,但没关系.
     */
    _db_writeidx(db, db->idxbuf, db->idxoff, SEEK_SET, freeptr);

    /*
     * 写入新的空闲列表指针.
     */
    _db_writeptr(db, FREE_OFF, db->idxoff);

    /*
     * 重写指向正在被删除的记录的链表指针.
     * 回想一下_db_find_and_lock设置了db->ptroff指向这个链表指针.
     * 我们将这个链表指针设置为已删除记录的链表指针的内容,saveptr.
     */

    // 将数据库的写指针设置为saveptr指向的位置
    _db_writeptr(db, db->ptroff, saveptr);

    // 尝试解锁数据库文件,如果解锁失败,则输出错误信息
    // db->idxfd: 数据库文件的文件描述符
    // FREE_OFF: 解锁的区域偏移量
    // SEEK_SET: 从文件开始位置进行查找
    // 1: 解锁的长度,这里表示解锁一个记录的长度
    if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("_db_dodelete: un_lock error");  // 如果解锁失败,调用err_dump函数输出错误信息
}

/*
 * 写入数据记录.被_db_dodelete(写入空白记录)和db_store调用.
 */
static void
_db_writedat(DB *db, const char *data, off_t offset, int whence)
{
    struct iovec iov[2];             // 定义两个iovec结构体,用于writev函数
    static char  newline = NEWLINE;  // 定义一个静态变量存储换行符

    /*
     * 如果我们正在追加数据,我们必须在做lseek和write之前加锁,
     * 以使这两个操作成为原子操作.如果我们正在覆盖一个现有的记录,
     * 我们不需要加锁.
     */
    // 如果是追加模式,锁定整个文件以防止其他进程同时写入
    if (whence == SEEK_END)
        // 使用writew_lock函数锁定文件,从文件开始位置(SEEK_SET)到文件末尾
        // 如果锁定失败,调用err_dump函数报告错误
        if (writew_lock(db->datfd, 0, SEEK_SET, 0) < 0)
            err_dump("_db_writedat: writew_lock error");

    // 使用lseek定位到指定的偏移量
    if ((db->datoff = lseek(db->datfd, offset, whence)) == -1)
        err_dump("_db_writedat: lseek error");
    db->datlen = strlen(data) + 1;  // 计算数据长度,包括换行符

    // 设置第一个iovec结构体,指向数据部分
    iov[0].iov_base = (char *)data;
    iov[0].iov_len  = db->datlen - 1;
    // 设置第二个iovec结构体,指向换行符
    iov[1].iov_base = &newline;
    iov[1].iov_len  = 1;
    // 使用writev函数同时写入数据和换行符
    if (writev(db->datfd, &iov[0], 2) != db->datlen)
        err_dump("_db_writedat: writev error of data record");

    // 如果是追加模式,解锁文件
    // 当whence等于SEEK_END时,表示文件打开模式为追加模式
    // 在追加模式下,需要先解锁文件,以便进行写操作
    if (whence == SEEK_END)
        // 调用un_lock函数解锁文件
        // db->datfd: 文件描述符
        // 0: 起始位置偏移量
        // SEEK_SET: 锁定起始位置相对于文件开始
        // 0: 锁定长度,0表示锁定到文件末尾
        if (un_lock(db->datfd, 0, SEEK_SET, 0) < 0)
            // 如果解锁失败,调用err_dump函数输出错误信息
            err_dump("_db_writedat: un_lock error");
}

/*
 * 写入索引记录.在调用此函数之前,应调用_db_writedat来设置DB结构中的datoff和datlen字段,
 * 这是我们需要写入索引记录的.
 */
static void
_db_writeidx(DB *db, const char *key, off_t offset, int whence, off_t ptrval)
{
    struct iovec iov[2];                               // 定义两个iovec结构体,用于writev函数
    char         asciiptrlen[PTR_SZ + IDXLEN_SZ + 1];  // 存储ptrval和idxbuf长度的ASCII字符串
    int          len;                                  // idxbuf的长度

    // 检查ptrval是否有效,并设置db->ptrval

    // 检查ptrval是否有效,并将其赋值给db->ptrval
    // 如果ptrval小于0或者大于PTR_MAX,则打印错误信息并退出程序
    if ((db->ptrval = ptrval) < 0 || ptrval > PTR_MAX)
        err_quit("_db_writeidx: invalid ptr: %d", ptrval);

    // 格式化索引缓冲区,包含key,分隔符,datoff,分隔符和datlen
    // 将键值,分隔符,数据偏移量和数据长度格式化到db->idxbuf中
    // %s 表示字符串,%c 表示字符,%lld 表示长整型(64位),%ld 表示长整型(32位)
    // \n 是换行符
    sprintf(db->idxbuf, "%s%c%lld%c%ld\n", key, SEP, (long long)db->datoff, SEP, (long)db->datlen);

    // 获取idxbuf的长度,strlen函数计算字符串的长度,不包括结束符'\0'
    len = strlen(db->idxbuf);

    // 检查长度是否在有效范围内
    if (len < IDXLEN_MIN || len > IDXLEN_MAX)
        err_dump("_db_writeidx: invalid length");

    // 格式化asciiptrlen,包含ptrval和idxbuf长度
    // sprintf函数用于将格式化的数据写入字符串.
    // %*lld 表示一个宽度为PTR_SZ的long long类型整数,*表示宽度由后面的参数指定.
    // %*d 表示一个宽度为IDXLEN_SZ的整数,*同样表示宽度由后面的参数指定.
    // ptrval是要转换的long long类型值,len是要转换的整数.
    // asciiptrlen是目标字符串,用于存储转换后的结果.
    // 该代码的目的是将ptrval和len的值按照指定的宽度格式化后,写入asciiptrlen字符串中.
    sprintf(asciiptrlen, "%*lld%*d", PTR_SZ, (long long)ptrval, IDXLEN_SZ, len);

    // 如果是追加操作,需要先加锁再进行lseek和writev,以保证原子操作
    // 如果whence等于SEEK_END,表示我们要追加数据
    if (whence == SEEK_END)
    {
        // 尝试对数据库文件进行写锁,锁定从(db->nhash + 1) * PTR_SZ + 1开始的区域
        // 这样做是为了防止在写入索引时,其他进程修改文件内容
        if (writew_lock(db->idxfd, ((db->nhash + 1) * PTR_SZ) + 1, SEEK_SET, 0) < 0)
            // 如果加锁失败,输出错误信息并终止程序
            err_dump("_db_writeidx: writew_lock error");
    }

    // 定位索引文件并记录偏移量
    // _db_writeidx: 在数据库中写入索引
    // db: 指向数据库结构的指针
    // offset: 文件中的偏移量
    // whence: 偏移的起始位置 (SEEK_SET, SEEK_CUR, SEEK_END)
    //
    // 该函数尝试将文件指针移动到指定的偏移量,以便写入索引.
    // 如果lseek调用失败(返回-1),则通过err_dump报告错误.
    if ((db->idxoff = lseek(db->idxfd, offset, whence)) == -1)
        err_dump("_db_writeidx: lseek error");

    // 设置iovec结构体,准备写入数据
    // 为选定的代码添加注释

    // iov[0] 的基地址设置为 asciiptrlen,长度设置为 PTR_SZ + IDXLEN_SZ
    // 这部分数据可能用于存储指向某种数据结构的指针及其长度信息
    iov[0].iov_base = asciiptrlen;
    iov[0].iov_len  = PTR_SZ + IDXLEN_SZ;

    // iov[1] 的基地址设置为 db 结构体中的 idxbuf 成员,长度设置为 len
    // 这部分数据可能用于存储数据库索引缓冲区的内容
    iov[1].iov_base = db->idxbuf;
    iov[1].iov_len  = len;

    // 写入索引记录
    // _db_writeidx: 将索引记录写入数据库文件
    // writev: 将多个缓冲区的数据写入一个文件描述符
    // iov: 包含要写入的数据的缓冲区数组
    // PTR_SZ + IDXLEN_SZ + len: 要写入的总字节数
    // err_dump: 如果写入操作失败,则打印错误信息并退出程序
    if (writev(db->idxfd, &iov[0], 2) != PTR_SZ + IDXLEN_SZ + len)
        err_dump("_db_writeidx: writev error of index record");

    // 如果是追加操作,写入完成后解锁
    // 如果whence等于SEEK_END,执行以下操作
    if (whence == SEEK_END)
        // 尝试解锁db->idxfd文件描述符,从((db->nhash + 1) * PTR_SZ) + 1偏移量开始,以SEEK_SET模式解锁,解锁长度为0
        // 如果解锁失败(返回值小于0),则调用err_dump函数输出错误信息"_db_writeidx: un_lock error"
        if (un_lock(db->idxfd, ((db->nhash + 1) * PTR_SZ) + 1, SEEK_SET, 0) < 0)
            err_dump("_db_writeidx: un_lock error");
}

/*
 * _db_writeptr - 将指针值写入数据库索引文件的指定偏移位置
 * @db: 数据库结构体指针
 * @offset: 要写入指针值的文件偏移量
 * @ptrval: 要写入的指针值
 *
 * 该函数用于将一个指针值以字符串形式写入数据库索引文件的指定偏移位置.
 * 指针值首先被转换为固定长度的字符串,然后使用lseek定位到文件的指定偏移,
 * 最后使用write函数将字符串写入文件.
 */
static void
_db_writeptr(DB *db, off_t offset, off_t ptrval)
{
    char asciiptr[PTR_SZ + 1];  // 定义一个字符数组用于存储转换后的指针值字符串

    // 检查指针值是否在有效范围内
    if (ptrval < 0 || ptrval > PTR_MAX)
        err_quit("_db_writeptr: invalid ptr: %d", ptrval);

    // 将指针值转换为固定长度的字符串
    sprintf(asciiptr, "%*lld", PTR_SZ, (long long)ptrval);

    // 使用lseek定位到文件的指定偏移
    if (lseek(db->idxfd, offset, SEEK_SET) == -1)
        err_dump("_db_writeptr: lseek error to ptr field");

    // 将字符串写入文件
    // _db_writeptr: 将指针字段写入数据库文件描述符
    // 参数:
    //   db: 数据库结构体指针
    //   asciiptr: 要写入的指针的ASCII表示形式的指针
    //   PTR_SZ: 指针字段的大小
    // 返回值:
    //   如果写入成功,无返回值;如果写入失败,调用err_dump函数报告错误
    if (write(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
        err_dump("_db_writeptr: write error of ptr field");
}

/*
 * Store a record in the database.  Return 0 if OK, 1 if record
 * exists and DB_INSERT specified, -1 on error.
 */

/*
 * 将记录存储到数据库中.如果操作成功返回0,如果指定了DB_INSERT且记录已存在则返回1,出错时返回-1.
 *
 * @param record 要存储的记录
 * @param flags 操作标志,例如DB_INSERT
 * @return 0表示成功,1表示记录已存在(当指定了DB_INSERT时),-1表示出错
 */

int db_store(DBHANDLE h, const char *key, const char *data, int flag)
{

    // 定义数据库指针
    DB *db = h;

    // 定义返回码,键长度,数据长度和指针值

    // 定义整型变量rc,用于存储函数返回值
    int rc;

    // 定义整型变量keylen,用于存储键的长度
    int keylen;

    // 定义整型变量datlen,用于存储数据的长度
    int datlen;

    // 定义off_t类型的变量ptrval,off_t通常用于表示文件中的偏移量
    off_t ptrval;

    // 检查标志位是否合法,如果不合法则设置errno并返回-1
    // 检查标志变量flag的值,确保它是DB_INSERT,DB_REPLACE或DB_STORE之一.
    // 如果不是这些值中的任何一个,将errno设置为EINVAL表示无效参数,并返回-1.
    if (flag != DB_INSERT && flag != DB_REPLACE &&
        flag != DB_STORE)
    {
        errno = EINVAL;  // 设置errno为EINVAL,表示传递了无效的参数.
        return (-1);     // 返回-1,表示函数执行出错.
    }

    // 计算键的长度
    keylen = strlen(key);

    // 计算数据的长度,加1是为了末尾的换行符
    datlen = strlen(data) + 1; /* +1 for newline at end */

    // 检查数据长度是否在允许的范围内,如果不在则输出错误信息
    if (datlen < DATLEN_MIN || datlen > DATLEN_MAX)
        err_dump("db_store: invalid data length");

    /*
     * _db_find_and_lock 用于计算新记录应该插入到哪个哈希表中(db->chainoff),
     * 不管该记录是否已经存在.接下来的 _db_writeptr 调用会改变这个哈希表条目,
     * 使其指向新记录.新记录被添加到哈希链的前端.
     */

    if (_db_find_and_lock(db, key, 1) < 0)
    { /* record not found */
        // 如果标志位为DB_REPLACE,表示尝试替换一个不存在的记录
        if (flag == DB_REPLACE)
        {
            rc = -1;            // 设置返回码为-1,表示操作失败
            db->cnt_storerr++;  // 增加存储错误的计数
            errno = ENOENT;     // 设置errno为ENOENT,表示记录不存在
            goto doreturn;      // 跳转到返回标签
        }

        /*
         * _db_find_and_lock 已经为我们锁定了哈希链;
         * 读取哈希链上第一个索引记录的链表指针.
         */
        ptrval = _db_readptr(db, db->chainoff);  // 读取链表指针

        // 如果找不到足够大的空记录
        if (_db_findfree(db, keylen, datlen) < 0)
        {
            /*
             * 无法找到足够大的空记录.将新记录追加到索引和数据文件的末尾.
             */
            // 将数据写入数据文件的末尾
            _db_writedat(db, data, 0, SEEK_END);
            // 将键写入索引文件的末尾,并获取新记录的指针值
            _db_writeidx(db, key, 0, SEEK_END, ptrval);

            /*
             * db->idxoff 由_db_writeidx设置.新记录放在哈希链的前面.
             */
            // 更新哈希链的头指针
            _db_writeptr(db, db->chainoff, db->idxoff);
            // 增加存储记录的计数
            db->cnt_stor1++;
        }

        else
        {
            /*
             * 重用一个空记录._db_findfree 已经将它从空闲列表中移除,并设置了 db->datoff 和 db->idxoff.
             * 重用的记录被放到哈希链的前端.
             */
            // 将数据写入到数据库的数据部分,偏移量为 db->datoff,起始位置为 SEEK_SET
            _db_writedat(db, data, db->datoff, SEEK_SET);
            // 将键写入到数据库的索引部分,偏移量为 db->idxoff,起始位置为 SEEK_SET,并关联 ptrval
            _db_writeidx(db, key, db->idxoff, SEEK_SET, ptrval);
            // 更新链表指针,将 db->chainoff 指向新的索引偏移量 db->idxoff
            _db_writeptr(db, db->chainoff, db->idxoff);
            // 增加存储操作计数器 db->cnt_stor2
            db->cnt_stor2++;
        }
    }
    else
    { /* record found */
        // 如果标志是DB_INSERT,表示尝试插入的记录已存在于数据库中
        if (flag == DB_INSERT)
        {
            rc = 1;             // 设置错误码,表示记录已存在
            db->cnt_storerr++;  // 增加存储错误的计数
            goto doreturn;      // 跳转到返回点
        }

        /*
         * 我们正在替换一个已存在的记录.我们知道新的键与现有的键相同,
         * 但我们还需要检查数据记录的大小是否相同.
         */

        if (datlen != db->datlen)
        {
            // 删除现有记录
            _db_dodelete(db);

            /*
             * 重新读取哈希表中的链表指针
             * (删除操作可能会导致其改变).
             */
            ptrval = _db_readptr(db, db->chainoff);

            /*
             * 将新的索引和数据记录追加到文件末尾.
             */
            _db_writedat(db, data, 0, SEEK_END);         // 写入数据记录
            _db_writeidx(db, key, 0, SEEK_END, ptrval);  // 写入索引记录

            /*
             * 新记录被放置到哈希链的前端.
             */
            _db_writeptr(db, db->chainoff, db->idxoff);  // 更新链表指针
            db->cnt_stor3++;                             // 增加存储操作计数
        }
        else
        {
            /*
             * 数据大小相同,仅替换数据记录.
             *
             * 函数: _db_writedat
             * 功能: 将数据写入数据库文件的指定偏移位置.
             * 参数:
             *   db - 数据库结构体指针
             *   data - 要写入的数据
             *   db->datoff - 数据开始的偏移量
             *   SEEK_SET - 文件指针定位到文件开头
             */
            _db_writedat(db, data, db->datoff, SEEK_SET);

            /*
             * 增加存储计数器,记录已经存储的数据条目数量.
             */
            db->cnt_stor4++;
        }
    }
    rc = 0; /* 初始化返回码为0,表示操作成功 */

doreturn: /* 标签,用于跳转到解锁哈希链的代码 */
    /* 尝试解锁之前通过_db_find_and_lock锁定的哈希链 */
    if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
        err_dump("db_store: un_lock error"); /* 如果解锁失败,打印错误信息并退出 */
    return (rc);                             /* 返回操作结果 */
}

/*
 * 尝试找到一个空闲的索引记录和相应大小的数据记录.
 * 我们只被 db_store 调用.
 */
static int
_db_findfree(DB *db, int keylen, int datlen)
{
    int   rc;                              // 返回码
    off_t offset, nextoffset, saveoffset;  // 偏移量

    /*
     * 锁定空闲列表.
     */
    if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("_db_findfree: writew_lock error");

    /*
     * 读取空闲列表指针.
     */
    saveoffset = FREE_OFF;
    offset     = _db_readptr(db, saveoffset);

    // 遍历空闲列表,寻找匹配大小的空闲记录
    while (offset != 0)
    {
        // 读取数据库索引,获取下一个偏移量
        nextoffset = _db_readidx(db, offset);
        // 如果找到匹配大小的空闲记录,则跳出循环
        // 这里检查当前索引缓冲区的字符串长度是否等于键的长度,并且数据长度等于期望的数据长度
        if (strlen(db->idxbuf) == keylen && db->datlen == datlen)
            break;
        // 如果没有找到匹配的记录,保存当前偏移量,并更新偏移量为下一个偏移量以继续搜索
        saveoffset = offset;
        offset     = nextoffset;
    }

    if (offset == 0)
    {
        rc = -1;  // 没有找到匹配的空闲记录
    }
    else
    {
        /*
         * 找到了一个匹配大小的空闲记录.
         * _db_readidx 上面已经读取了索引记录,并设置了 db->ptrval.
         * 同时,saveoffset 指向了指向这个空记录的空闲链表指针.
         * 我们将这个链表指针设置为 db->ptrval,从而从空闲列表中移除这个空记录.
         */
        _db_writeptr(db, saveoffset, db->ptrval);
        rc = 0;

        /*
         * 注意 _db_readidx 同时设置了 db->idxoff 和 db->datoff.
         * 这被调用者 db_store 使用,来写入新的索引记录和数据记录.
         */
    }

    /*
     * 解锁空闲列表.
     */
    // _db_findfree: 尝试解锁数据库文件描述符,如果失败则打印错误信息并退出
    // db->idxfd: 数据库文件的文件描述符
    // FREE_OFF: 解锁的起始偏移量
    // SEEK_SET: 从文件开始位置进行解锁
    // 1: 解锁的长度,这里表示解锁一个记录的长度
    if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("_db_findfree: un_lock error");  // 如果解锁失败,打印错误信息并退出程序

    return (rc);  // 返回结果码
}

/*
 * 重置索引文件以便db_nextrec使用.
 * 在db_open中自动调用.
 * 在第一次调用db_nextrec之前必须调用.
 */
void db_rewind(DBHANDLE h)
{
    DB   *db = h;  // 将传入的句柄转换为DB结构体指针
    off_t offset;  // 定义文件偏移量

    // 计算索引记录的起始位置,考虑了哈希表和空闲列表指针的大小
    offset = (db->nhash + 1) * PTR_SZ;

    /*
     * 我们只是为这个进程设置文件偏移量到索引记录的开始位置;
     * 不需要锁定.
     * 下面的+1是为了哈希表末尾的换行符.
     */
    // 将文件偏移量设置为计算出的索引记录起始位置,并检查是否成功
    if ((db->idxoff = lseek(db->idxfd, offset + 1, SEEK_SET)) == -1)
        err_dump("db_rewind: lseek error");  // 如果lseek失败,打印错误信息
}
/*
 * 返回下一个顺序记录.
 * 我们只是简单地遍历索引文件,忽略已删除的记录.
 * 在第一次调用此函数之前,必须调用db_rewind.
 */

// 函数:db_nextrec
// 功能:返回数据库中的下一个记录
// 参数:
//   h - 数据库句柄
//   key - 指向用于存储记录键的字符数组的指针
// 返回值:指向数据缓冲区的指针,如果到达文件末尾则返回NULL
char *
db_nextrec(DBHANDLE h, char *key)
{
    DB   *db = h;  // 将数据库句柄转换为DB结构体指针
    char  c;       // 用于检查记录键是否为空白字符的变量
    char *ptr;     // 用于遍历和返回数据的指针

    // 对空闲列表进行读锁定,以防止在删除记录的过程中读取记录
    // db_nextrec 函数中,尝试对数据库文件 db->idxfd 进行读锁定
    // 锁定的起始位置是 FREE_OFF,锁定长度为 1 个记录
    // 如果锁定失败,调用 err_dump 函数输出错误信息
    if (readw_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("db_nextrec: readw_lock error");

    do
    {
        // 读取下一个顺序索引记录
        if (_db_readidx(db, 0) < 0)
        {
            ptr = NULL;  // 索引文件结束,返回EOF
            goto doreturn;
        }

        // 检查键是否全部为空白(空记录)
        ptr = db->idxbuf;
        while ((c = *ptr++) != 0 && c == SPACE)
            ;  // 跳过直到空字节或非空白字符

    } while (c == 0);  // 循环直到找到非空白键

    // 如果key不为NULL,则将键复制到key指向的数组中
    if (key != NULL)
        strcpy(key, db->idxbuf);
    ptr = _db_readdat(db);  // 返回指向数据缓冲区的指针
    db->cnt_nextrec++;      // 增加下一个记录的计数

doreturn:
    // 对空闲列表进行解锁
    if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
        err_dump("db_nextrec: un_lock error");
    return (ptr);  // 返回指向数据缓冲区的指针
}
