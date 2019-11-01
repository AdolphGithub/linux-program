### 内存管理
```
#include <stdlib.h>

void *malloc(size_t size);  // 内存分配.返回的内存是地址对齐的.
```
当linxu物理内存耗尽,便会使用linux的交换空间(swap space).这个空间是在安装系统的时候分配的独立的磁盘区域.在linux中,所有的内存都以页的为单位进行划分.通常每一页的大小为4096字节(2MB).
```
void free(void *ptrto memory)   // 释放由malloc,calloc,realloc调用所分配的内存;
```
其他内存函数可以查看[内存分配](https://github.com/AdolphGithub/modern_c/tree/master/chapter/%E5%86%85%E5%AD%98%E5%88%86%E9%85%8D)
### 文件锁定
文件锁定可以阻止其他用户在同一时间对同一个文件进行写入操作.在使用open函数时加上对应的标志位即可.例如
```
int file_desc = open("/tmp/LCK.test", O_RDWR | O_CREAT | O_EXCL, 0444);
```
以上实现了对文件的索引,但对于大型的应用程序来说.有许多不同的程序负责对这个文件同时更新.如果使用文件锁机制就太慢了.linux提供了区域锁定的方式.就是锁定文件的部分区域.可以使用fcntl或lockf来实现.函数原型如下.
```
#include <fcntl.h>

int fcntl(int fildes, int command, ...)
// 其中command有3个用户文件锁定的命令选项.许多不同的进程可以拥有文件同一(或者重叠)区域上的共享锁.
// F_GETLK  用于获取fildes(第一个参数)打开的文件锁的信息.不会尝试去锁定文件
// F_SETLK  加锁操作.如果加锁成功,则返回一个非-1的值.
// F_SETLKW 等待获取锁的信息. 如果没有锁的信息会一直等待.
// 使用以上参数时,第三个参数必须是一个指向flock结构的指针. flock文件锁的接口依赖具体的实现.至少包含如下成员


struct flock {
    short l_type;  // F_RDLCK 共享(或读)锁, F_UNLCK 解锁, F_WRLCK 独占(或写)锁
    short l_whence;// 位于文件头(SEEK_SET),当前位置(SEEK_CUR),独占锁(SEEL_END)
    off_t l_start; // 开始位置
    off_t l_len;   // 长度
    pid_t l_pid;   // 用来记录锁的进程.
}
```
如果对文件进行了加锁,请使用底层的read和write来调用和访问文件中的数据.而不要使用更高级的fwrite和fread.除了fcntl还有一个lockf,原型如下
```
int lockf(int fileds, int function, off_t size_to_lock);
// function的参数的值如下.
// F_ULOCK   解锁
// F_LOCK    设置独占锁
// F_TLOCK   测试并设置独占锁
// F_TEST    测试其他进程设置的锁
// size_to_lock参数是操作的字节数,它从文件的当前偏移值开始计算.
```
在锁定文件时要注意死锁.假设
```
有两个程序想要更新同一个文件,例如程序A更新文件中的字节1和字节2,程序B更新文件中的这两个字节.程序A先锁定了字节1然后进行更新,程序B锁定了字节2进行更新.然后程序A尝试锁定字节2,但这时程序B也尝试锁定字节1.这个时候由于都被对方上了锁.所以两个都在等待就造成了死锁.
```
### dbm数据库
dbm数据库是一个索引化的文件存储系统,但X/Open规范把dbm看作是一个数据库.一般的linux不会自动安装.所以需要采用如下命令进行安装.
```
// ubuntu
sudo apt install libgdbm-dev
```
使用时要加上编译参数.-I/usr/include/gdbm或者-lgdbm.使用如下.
```
#include <ndbm.h>
```
ndbm.h包含一个新的数据类型.如下
```
typedef struct datum {
    void *dptr;     // dptr是数据的起点.
    size_t dsize;   // 表示数据的长度
}
```
当打开dbm数据库时,通常会创建两个文件.他们的后缀通常是.pag和.dir.包含的函数如下.
```
#include <ndbm.h>
// 访问函数. 
DBM *dbm_open(const char *filename,int file_open_flags, mode_t file_mode);  // 打开已有数据库,或创建新的数据库.file_open_flags和file_mode跟open一样.
int dbm_store(DBM *database_desriptor, datum key,datum content, int store_mode); // 存储数据,其中key为索引,content为内容.操作模式,例如dbm_insert(插入),dbm_replace(更新)
datum dbm_fetch(DBM *database_descriptor, datum key); // 查找一行数据.如果未查找到,则datum的dptr指针为空.这里是指针,所以要操作的化需要重新复制一份数据出来.
void dbm_close(DBM *database_descriptor); // 关闭打开的dbm数据库.
```