/*
 * Provide an simpler and easier to understand interface to the System V
 * semaphore system calls.  There are 7 routines available to the user:
 *
 *	id = sem_create(key, initval);	# create with initial value or open
 *	id = sem_open(key);		# open (must already exist)
 *	sem_wait(id);			# wait = P = down by 1
 *	sem_signal(id);			# signal = V = up by 1
 *	sem_op(id, amount);		# wait   if (amount < 0)
 *					# signal if (amount > 0)
 *	sem_close(id);			# close
 *	sem_rm(id);			# remove (delete)
 *
 * We create and use a 3-member set for the requested semaphore.
 * The first member, [0], is the actual semaphore value, and the second
 * member, [1], is a counter used to know when all processes have finished
 * with the semaphore.  The counter is initialized to a large number,
 * decremented on every create or open and incremented on every close.
 * This way we can use the "adjust" feature provided by System V so that
 * any process that exit's without calling sem_close() is accounted
 * for.  It doesn't help us if the last process does this (as we have
 * no way of getting control to remove the semaphore) but it will
 * work if any process other than the last does an exit (intentional
 * or unintentional).
 * The third member, [2], of the semaphore set is used as a lock variable
 * to avoid any race conditions in the sem_create() and sem_close()
 * functions.
 */
/*
 * 提供一个更简单,更容易理解的接口来使用 System V 信号量系统调用.
 * 用户可以使用以下 7 个例程:
 *
 *	id = sem_create(key, initval);	# 创建具有初始值或打开
 *	id = sem_open(key);		# 打开(必须已存在)
 *	sem_wait(id);			# 等待 = P = 减 1
 *	sem_signal(id);			# 信号 = V = 加 1
 *	sem_op(id, amount);		# 如果 (amount < 0) 则等待
 *					# 如果 (amount > 0) 则信号
 *	sem_close(id);			# 关闭
 *	sem_rm(id);			# 移除(删除)
 *
 * 我们创建并使用一个包含 3 个成员的集合来请求信号量.
 * 第一个成员 [0] 是实际的信号量值,第二个成员 [1] 是一个计数器,
 * 用来知道所有进程何时完成对信号量的使用.
 * 计数器初始化为一个很大的数字,在每次创建或打开时递减,在每次关闭时递增.
 * 这样我们就可以使用 System V 提供的"调整"功能,
 * 以便任何未调用 sem_close() 就退出的进程都能被计算在内.
 * 如果最后一个进程这样做(我们没有办法获得控制权来移除信号量)则没有帮助,
 * 但如果除了最后一个进程之外的任何进程退出(无论是故意的还是非故意的),它都会起作用.
 * 信号量集合的第三个成员 [2] 用作锁变量,
 * 以避免 sem_create() 和 sem_close() 函数中的任何竞态条件.
 */

#include "apue.h"     // 引入APUE库的头文件
#include <sys/ipc.h>  // 引入IPC机制相关的头文件
#include <sys/sem.h>  // 引入信号量相关的头文件
#include <errno.h>    // 引入错误处理相关的头文件

// 函数声明
// 对信号量执行操作的函数,具体操作由第二个参数决定
void sem_op(int, int);
// 创建一个新的信号量集的函数,返回信号量集的ID
int sem_create(key_t, int);
// 打开一个已存在的信号量集的函数,返回信号量集的ID
int sem_open(key_t);
// 删除一个信号量集的函数
void sem_rm(int);
// 关闭信号量集的函数
void sem_close(int);
// 等待信号量的函数,即P操作
void sem_wait(int);
// 发送信号量的函数,即V操作
void sem_signal(int);

// 定义信号量操作数组,用于semop()调用.

// op_lock用于锁定信号量,等待信号量[2]等于0,然后将其增加到1以锁定
static struct sembuf op_lock[2] = {
    {2, 0, 0},        // 等待[2](锁)等于0
    {2, 1, SEM_UNDO}  // 然后将[2]增加到1 - 这会锁定它
                      // 如果进程退出前没有明确解锁,则UNDO用于释放锁
};

// op_endcreate用于结束创建信号量,减少进程计数器[1],并在退出时使用UNDO调整计数器
// 然后将锁[2]减少回0
static struct sembuf op_endcreate[2] = {
    {1, -1, SEM_UNDO},  // 减少[1](进程计数器)并在退出时使用UNDO
                        // 如果进程退出前没有明确调用sem_close(),则UNDO用于调整进程计数器
    {2, -1, SEM_UNDO}   // 然后将[2](锁)减少回0
};

// op_open用于打开信号量,减少进程计数器[1],并在退出时使用UNDO
static struct sembuf op_open[1] = {
    {1, -1, SEM_UNDO}  // 减少[1](进程计数器)并在退出时使用UNDO
};

// op_close用于关闭信号量,等待锁[2]等于0,然后将其增加到1以锁定
// 然后增加进程计数器[1]
static struct sembuf op_close[3] = {
    {2, 0, 0},         // 等待[2](锁)等于0
    {2, 1, SEM_UNDO},  // 然后将[2]增加到1 - 这会锁定它
    {1, 1, SEM_UNDO}   // 然后增加[1](进程计数器)
};

// op_unlock用于解锁信号量,将锁[2]减少回0
static struct sembuf op_unlock[1] = {
    {2, -1, SEM_UNDO}  // 将[2](锁)减少回0
};

// op_op用于对信号量[0]进行增减操作,99表示实际要增加或减少的数量(正数或负数)
static struct sembuf op_op[1] = {
    {0, 99, SEM_UNDO}  // 对[0]进行增减操作,并在退出时使用UNDO
                       // 99设置为实际要增加或减少的数量
};
/****************************************************************************
 * 创建一个具有指定初始值的信号量.
 * 如果信号量已存在,我们不进行初始化(当然).
 * 如果一切正常,我们返回信号量ID,否则返回-1.
 */

// 定义sem_create函数,用于创建信号量
int sem_create(key_t key, int initval)
{
    register int id, semval;
    // 定义联合体semun,用于semctl函数的参数
    union semun
    {
        int              val;    // 用于SETVAL操作
        struct semid_ds *buf;    // 用于IPC_STAT和IPC_SET操作
        unsigned short  *array;  // 用于GETALL和SETALL操作
    } semctl_arg;

    // 如果key为IPC_PRIVATE,则返回-1,因为不适用于私有信号量
    if (key == IPC_PRIVATE)
        return (-1);

    // 如果key为(key_t)-1,则返回-1,可能是调用者ftok()函数出错
    else if (key == (key_t)-1)
        return (-1);

again:
    // 使用semget函数尝试获取或创建信号量集,如果失败则返回-1
    if ((id = semget(key, 3, 0666 | IPC_CREAT)) < 0)
        return (-1);

    // 尝试锁定信号量,如果失败则根据错误类型处理
    if (semop(id, &op_lock[0], 2) < 0)
    {
        if (errno == EINVAL)
            goto again;         // 如果信号量ID无效,重新尝试创建
        err_sys("can't lock");  // 其他错误,输出错误信息
    }

    // 获取信号量集中的值,如果失败则输出错误信息
    if ((semval = semctl(id, 1, GETVAL, 0)) < 0)
        err_sys("can't GETVAL");

    // 如果信号量值为0,表示尚未初始化
    if (semval == 0)
    {
        // 初始化信号量的两个成员
        semctl_arg.val = initval;
        if (semctl(id, 0, SETVAL, semctl_arg) < 0)
            err_sys("can SETVAL[0]");

        semctl_arg.val = BIGCOUNT;
        if (semctl(id, 1, SETVAL, semctl_arg) < 0)
            err_sys("can SETVAL[1]");
    }

    // 减少进程计数器并释放锁
    if (semop(id, &op_endcreate[0], 2) < 0)
        err_sys("can't end create");

    // 返回信号量ID
    return (id);
}

/****************************************************************************
 * 打开一个必须已经存在的信号量.
 * 如果调用者知道信号量必须已经存在,则应使用此函数,而不是sem_create().
 * 例如,客户端-服务器对中的客户端会使用这个函数,如果创建信号量是服务器的责任.
 * 如果一切正常,我们返回信号量ID,否则返回-1.
 */

int sem_open(key_t key)
{
    register int id;

    // 如果key是IPC_PRIVATE,则不适用于私有信号量
    if (key == IPC_PRIVATE)
        return (-1);

    // 如果key是(key_t)-1,则可能是调用者ftok()出错
    else if (key == (key_t)-1)
        return (-1);

    // 尝试获取信号量集ID,如果不存在或表已满,则返回-1
    if ((id = semget(key, 3, 0)) < 0)
        return (-1);

    /*
     * 减少进程计数器.我们不需要锁就可以做这个操作.
     */

    // 执行semop操作来打开信号量,如果失败则调用err_sys()报告错误
    if (semop(id, &op_open[0], 1) < 0)
        err_sys("can't open");

    // 返回信号量ID
    return (id);
}

/****************************************************************************
 * 移除一个信号量.
 * 此调用旨在由服务器调用,例如,在关闭时,我们对信号量执行 IPC_RMID,
 * 而不管其他进程是否可能在使用它.
 * 大多数其他进程应该使用下面的 sem_close().
 */

/**
 * @brief 移除指定的信号量.
 *
 * 此函数尝试移除一个信号量,通常在服务器关闭时调用.
 * 它会执行 IPC_RMID 操作,这个操作会立即移除信号量,
 * 不考虑其他进程是否仍在使用它.
 *
 * @param id 要移除的信号量的标识符.
 * @return void
 */
void sem_rm(int id)
{
    // 尝试执行 IPC_RMID 操作
    if (semctl(id, 0, IPC_RMID, 0) < 0)
        // 如果操作失败,打印错误信息并退出
        err_sys("can't IPC_RMID");
}

/****************************************************************************
 * 关闭信号量.
 * 与上面的remove函数不同,此函数是供进程在退出前,当它完成对信号量的使用时调用的.
 * 我们"减少"使用信号量的进程计数器,如果这是最后一个,则可以移除信号量.
 */

void sem_close(int id)
{
    register int semval;

    /*
     * 下面的semop()首先获取信号量的锁,
     * 然后增加[1] - 进程计数器.
     */

    // 尝试获取信号量锁并增加进程计数器
    if (semop(id, &op_close[0], 3) < 0)
        err_sys("can't semop");  // 如果失败,输出错误信息

    /*
     * 现在我们有了锁,读取进程计数器的值,看看这是否是对信号量的最后一个引用.
     * 这里有一个竞态条件 - 见sem_create()中的注释.
     */

    // 获取进程计数器的值
    if ((semval = semctl(id, 1, GETVAL, 0)) < 0)
        err_sys("can't GETVAL");  // 如果失败,输出错误信息

    // 检查进程计数器的值是否合理
    if (semval > BIGCOUNT)
        err_dump("sem[1] > BIGCOUNT");        // 如果计数器值过大,输出错误信息
    else if (semval == BIGCOUNT)              // 如果是最后一个引用
        sem_rm(id);                           // 移除信号量
    else                                      // 如果还有其他引用
        if (semop(id, &op_unlock[0], 1) < 0)  // 尝试解锁
            err_sys("can't unlock");          // 如果失败,输出错误信息
}

/****************************************************************************
 * 等待信号量的值大于0,然后将其减1并返回.
 * 这是Dijkstra的P操作,也是Tanenbaum的DOWN操作.
 */

// sem_wait函数用于等待信号量,直到其值大于0,然后将其减1
// 参数:
//   id: 信号量的标识符
void sem_wait(int id)
{
    // 调用sem_op函数,传入信号量id和减去的值-1
    sem_op(id, -1);
}

/****************************************************************************
 * 增加信号量的值.
 * 这是Dijkstra的V操作,也是Tanenbaum的UP操作.
 */

// sem_signal函数用于增加指定信号量的值
// 参数:
//   id - 信号量的标识符
void sem_signal(int id)
{
    // 调用sem_op函数,传入信号量id和增量1
    sem_op(id, 1);
}

/****************************************************************************
 * 通用信号量操作.按用户指定的数量(正数或负数;数量不能为零)递增或递减.
 */

// sem_op函数用于对信号量进行操作,包括增加或减少指定的值
// id: 信号量的标识符
// value: 需要增加或减少的值
void sem_op(int id, int value)
{
    // 检查value是否为0,如果是则报错,因为信号量的操作值不能为0
    // 如果value为0,则赋值给op_op[0].sem_op后,条件表达式结果也为0,触发err_sys报错
    if ((op_op[0].sem_op = value) == 0)
        err_sys("can't have value == 0");

    // 调用semop函数执行信号量操作,如果操作失败则报错
    // semop函数用于执行信号量集合上的操作,如果返回值小于0,表示操作失败,触发err_sys报错
    if (semop(id, &op_op[0], 1) < 0)
        err_sys("sem_op error");
}
