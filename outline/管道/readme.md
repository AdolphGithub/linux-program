### 管道
当一个进程连接数据流到另一个进程时,使用管道.在shell命令中管道定义如下
```
cmd1 | cmd2
```
### 进程管道
最简单在两个程序之间传递数据的方法就是使用popen和pclose函数.原型如下
```
#include <stdio.h>

// 打开一个新的进程来启动.并可以传递数据给它.并设置打开的模式.
FILE *popen(const char *command,const char *open_mode);
// 关闭打开的进程.只有等待进程结束之后才有返回.返回值为关闭进程的退出码.
int pclose(FILE *stream_to_close);
```
在popen调用运行一个程序时,会首先启动shell及sh命令.然后将command字符串作为一个参数传递给它.所以一旦调用popen函数则会启用两个甚至更多的进程.从节省系统资源的角度来看,成本略高.
### pipe调用
pipe函数跟popen最大的区别在于pipe函数不需要通过调用shell来启动一个程序,函数原型如下
```
#include <unistd.h>

// 参数是由两个整数类型的文件描述符组成的数组的指针.如果成功返回0,失败返回-1.
// 这里的数据通信采用的是先进先出原则.如果对file_descriptor[1]写入1,2,3
// 从file_descriptor[0]中读取出来的顺序也是1,2,3
int pipe(int file_descriptor[2]);
```
注意在父进程中调用pipe会自动关联子进程.在操作管道时,使用原始的write和read操作.因为保存的是文件操作符.
### 将管道用作标准输入和标准输出
```
#include <unistd.h>

int dup(int file_descriptor);
int dup2(int file_desccriptor_one, int file_descriptor_two);
```
dup调用的目的是打开一个新的文件描述符.dup创建的新文件描述符与作为它的参数的那个已有文件描述符指向同一个文件(或管道).对于dup函数来说.文件操作符总是取最小的可用值.如果我们关闭标准输入文件描述符0,在使用dup则会将标准输入文件描述符改为我们自己创建的文件操作描述符.

### 命名管道: FIFO
linux中所有进程都有一个祖先进程(/sbin/init)统一管理.但我们想在不相关的进程之间交换数据．这还不是很方便.可以通过FIFO来完成这项工作.
```
// 命名管道是linux中一个特殊类型的文件.以文件名形式存在.可以使用mknod来创建.如下

mknod filename p
```
程序中使用如下命令
```
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *filename, mode_t mode);
int mknod(const char *filename, mode_t mode | S_IFIFO, (dev_t) 0);
```
