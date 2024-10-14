#include <limits.h>   // 引入标准库,定义了各种类型的极限值
#include <string.h>   // 引入字符串处理函数
#include <pthread.h>  // 引入POSIX线程库
#include <stdlib.h>   // 引入标准库,包含内存分配和程序终止等功能

#define MAXSTRINGSZ 4096  // 定义一个宏,表示字符串的最大长度

// 定义线程特定数据键
static pthread_key_t key;
// 定义线程初始化控制变量
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
// 定义互斥锁,用于保护环境变量的访问
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

// 引入环境变量数组
extern char **environ;

// 线程初始化函数,创建线程特定数据键
static void
thread_init(void)
{
    // 创建一个线程特定数据键,关联的析构函数为free,用于在线程退出时释放相关资源
    pthread_key_create(&key, free);
}

/**
 * @brief 获取指定名称的环境变量的值
 *
 * @param name 环境变量的名称
 * @return char* 返回环境变量的值,如果未找到则返回NULL
 */
char *
getenv(const char *name)
{
    int   i, len;
    char *envbuf;

    // 确保线程初始化函数只被调用一次
    pthread_once(&init_done, thread_init);
    // 加锁,保护环境变量的访问
    pthread_mutex_lock(&env_mutex);
    // 获取当前线程的环境变量缓冲区
    envbuf = (char *)pthread_getspecific(key);
    if (envbuf == NULL)
    {
        // 如果缓冲区为空,则分配内存
        envbuf = malloc(MAXSTRINGSZ);
        if (envbuf == NULL)
        {
            // 如果内存分配失败,解锁并返回NULL
            // 解锁环境变量的互斥锁
            // 如果之前有锁定操作,这里会释放锁,允许其他线程访问共享资源
            // 注意:这里假设env_mutex已经被正确初始化
            pthread_mutex_unlock(&env_mutex);

            // 返回NULL,可能表示操作失败或者没有找到环境变量
            return (NULL);
        }
        // 设置当前线程的环境变量缓冲区
        pthread_setspecific(key, envbuf);
    }
    // 计算环境变量名称的长度
    len = strlen(name);
    // 遍历环境变量数组
    // 遍历环境变量数组
    for (i = 0; environ[i] != NULL; i++)
    {
        // 检查是否找到匹配的环境变量
        // strncmp用于比较name和环境变量名称的前len个字符是否相同
        // environ[i][len] == '=' 表示名称后紧跟着等号,符合环境变量的格式
        if ((strncmp(name, environ[i], len) == 0) &&
            (environ[i][len] == '='))
        {
            // 复制环境变量的值到缓冲区
            // 从等号后的第一个字符开始复制,直到缓冲区大小减去1的位置
            strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
            // 确保字符串以空字符结尾
            envbuf[MAXSTRINGSZ - 1] = '\0';
            // 解锁并返回环境变量的值
            // 假设env_mutex是一个已经初始化的互斥锁,用于保护环境变量的访问
            pthread_mutex_unlock(&env_mutex);
            return (envbuf);  // 返回找到的环境变量的值
        }
    }

    // 如果没有找到匹配的环境变量,解锁并返回NULL
    pthread_mutex_unlock(&env_mutex);
    return (NULL);
}
