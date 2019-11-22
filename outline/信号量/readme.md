### 信号量定义
一个特殊变量，只允许对它进行等待(wait)和发送信号两种操作
### 信号量基本函数
```
#include <sys/sem.h>

// 直接控制信号量信息
// sem_id是由semget返回的信号量标识符
// sem_num参数是信号量编号
// command是将要采取的动作.
// 第四个参数可以为semun的联合体.要自己定义.详细可查看手册.
int semctl(int sem_id, int sem_num, int command, ...);
// 创建一个新信号量或取得一个已有的信号量的键,
// key是整数值,不相关的进程可以通过它访问同一个信号量.
// num_sems是指定需要信号量的数目.通常是1.
// sem_flags是一组标志.它与open还是的标志非常相似．
// 成功时返回一个正数(非0)值.就是其他信号量函数将用到的信号量标识符.
int semget(key_t key, int num_sems, int sem_flags);
// 改变信号量的值.
// sem_id是由semget返回的信号量标识符
// sem_ops是指向sembuf结构指针.
// sembuf至少包含3个参数,sem_num,sem_op,sem_flg.sem_num是信号量编号,sem_op是信号量在一次操作中需要改变的值.sem_flg通常被设置为SEM_UNDO.
int semop(int sem_id, struct sembuf *sem_ops, size_t num_sem_ops);
```
### 共享内存
用于不同进程之间对某些公共数据来进行数据交互.常用函数
```
#include <sys/types.h>
#include <sys/shm.h>

// 连接到一个进程的地址空间.并启用.
// shm_id由shmget返回的共享内存标识符.
// shm_addr指定的是共享内存连接到当前进程中的地址位置.通常是一个NULL指针.
// shmflg是一组位标志.可能取值为SHM_RND(控制共享内存连接的地址)和SHM_RDONLY(只读)
// 调用成功,它返回一个指向共享内存第一个字节的指针.失败返回-1
void *shmat(int shm_id,const void *shm_addr, int shmflg);
// 共享内存控制函数
// shm_id是shmget返回的共享内存标识符
// command是将要采取的动作,可以取3个值,
// IPC_STAT(把shmid_ds结构中的数据设置为共享内存的当前关联值)
// IPC_SET(如果进程有足够的权限.就把共享内存的当前关联值设置为shmid_ds结构中给出的值)
// IPC_RMID 删除共享内存段
// shmid_ds结构体请查看man 2 shmctl
int shmctl(int shm_id, int cmd, struct shmid_ds *buf);
// 将共享内存从当前进程中分离.shm_addr是由shmat返回的指针.
int shmdt(const void *shm_addr);
// 创建共享内存
// key用来有效的为共享内存段命名
// size指定需要共享内存的容量
// shmflg包含9个比特的权限标志.作用与文件创建时使用的mode标志一样.
// 如果创建成功,则返回一个非负整数.即共享内存标识符.失败就返回-1.创建后无法使用.还需要使用shmat来处理.
int shmget(key_t key, size_t size, int shmflg);
```