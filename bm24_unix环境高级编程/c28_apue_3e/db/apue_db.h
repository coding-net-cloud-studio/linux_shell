// apue_db.h

#ifndef _APUE_DB_H
#define _APUE_DB_H

// 定义数据库句柄类型
typedef void *DBHANDLE;

/**
 * 打开数据库
 * @param path 数据库路径
 * @param oflag 打开标志
 * @param ... 其他参数
 * @return 数据库句柄
 */
DBHANDLE db_open(const char *, int, ...);

/**
 * 关闭数据库
 * @param db 数据库句柄
 */
void db_close(DBHANDLE);

/**
 * 获取记录
 * @param db 数据库句柄
 * @param key 记录的键
 * @return 记录的值
 */
char *db_fetch(DBHANDLE, const char *);

/**
 * 存储记录
 * @param db 数据库句柄
 * @param key 记录的键
 * @param data 记录的值
 * @param flag 存储标志 (DB_INSERT, DB_REPLACE, DB_STORE)
 * @return 成功返回0,失败返回非0
 */
int db_store(DBHANDLE, const char *, const char *, int);

/**
 * 删除记录
 * @param db 数据库句柄
 * @param key 记录的键
 * @return 成功返回0,失败返回非0
 */
int db_delete(DBHANDLE, const char *);

/**
 * 回退到数据库的开始位置
 * @param db 数据库句置
 */
void db_rewind(DBHANDLE);

/**
 * 获取下一条记录
 * @param db 数据库句柄
 * @param key 用于存储下一条记录键的缓冲区
 * @return 记录的值
 */
char *db_nextrec(DBHANDLE, char *);

/*
 * db_store() 的标志定义
 */
#define DB_INSERT  1 /* 仅插入新记录 */
#define DB_REPLACE 2 /* 替换现有记录 */
#define DB_STORE   3 /* 替换或插入 */

/*
 * 实现限制
 */
#define IDXLEN_MIN 6    /* 键,分隔符,开始,分隔符,长度,\n */
#define IDXLEN_MAX 1024 /* 任意 */
#define DATLEN_MIN 2    /* 数据字节,换行 */
#define DATLEN_MAX 1024 /* 任意 */

#endif /* _APUE_DB_H */
