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
// F_GETLK  用于获取fildes(第一个餐数)打开的文件锁的信息.不会尝试去锁定文件
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
