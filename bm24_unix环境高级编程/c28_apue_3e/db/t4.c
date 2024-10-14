#include "apue.h"     // 引入APUE库的头文件,提供系统调用和C标准库函数的封装
#include "apue_db.h"  // 引入APUE数据库操作的头文件,提供数据库操作的接口
#include <fcntl.h>    // 引入文件控制操作的头文件,提供文件打开,关闭等操作的函数原型

// 主函数
int main(void)
{
    DBHANDLE db;  // 定义数据库句柄

    // 打开或创建数据库文件,如果失败则输出错误信息并退出
    if ((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == NULL)
        err_sys("db_open error");

    // 向数据库中插入数据,如果失败则输出错误信息并退出

    // 存储键值对到数据库中,如果键已经存在则返回错误
    // db: 数据库句柄
    // key: 要存储的键
    // data: 要存储的数据
    // flags: 操作标志,DB_INSERT 表示如果键已存在则返回错误
    // 返回值: 成功返回0,失败返回非0
    if (db_store(db, "Alpha", "data1", DB_INSERT) != 0)
        err_quit("db_store error for alpha");  // 如果存储失败,打印错误信息并退出

    // 存储键值对到数据库中,如果键已经存在则返回错误
    if (db_store(db, "beta", "Data for beta", DB_INSERT) != 0)
        err_quit("db_store error for beta");  // 如果存储失败,打印错误信息并退出

    // 存储键值对到数据库中,如果键已经存在则返回错误
    if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
        err_quit("db_store error for gamma");  // 如果存储失败,打印错误信息并退出

    // 关闭数据库连接
    db_close(db);
    // 正常退出程序
    exit(0);
}
