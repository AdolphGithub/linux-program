### 什么是线程
线程是进程中的一个控制序列.线程与线程的创建者共享全局变量,文件描述符,信号处理函数和当前目录状态.
### 线程的优点和缺点
```
// 优点
线程创建比进程创建所耗资源更少.
线程切换比进程切换要块.
// 缺点
比单线程程序的调试困难得多.
编写多线程程序需要非常仔细的设计.
```
基本调用
```
#include <pthread.h>

// 线程创建.第一个参数为进程的指针,第二个参数为进程的属性(一般设置为NULL).第三个参数为一个参数和返回数都为void *的函数指针.第四个参数为传递为函数指针的参数.函数调用成功时返回0.失败则返回错误码.
int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*strat_routine)(void *), void *arg);
// 终止线程
void pthread_exit(void *retval);
// 进程中用来收集子线程信息的wait函数.第一个参数为将要等待的线程,指向线程返回值的指针.函数调用成功返回0.失败则返回错误码.
int pthread_join(pthread_t th, void **thread_return);
```