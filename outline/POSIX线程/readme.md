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
### 信号量
线程中,如果对共享变量保证原子操作.则可以使用互斥量.对一组相同对象访问时,就要使用信号量.信号量一般常用来保护一段代码,使其每次只能被一个执行线程运行.信号量的常用函数如下
```
#include <semaphore.h>

// 第一个参数为sem的信号量对象,pshared参数控制信号量的类型.如果值为0,就表示这个信号量是当前进程的局部信号量.否则,信号量就可以在多个进程之间共享.value为信号量的值
int sem_init(sem_t *sem, int pshared, unsigned int value);
// 以原子操作的方式给信号量的值加1
int sem_post(sem_t *sem);
// 以原子操作的方式给信号量的值减1,但是,如果信号量的值为0,不会开始减法操作
int sem_wait(sem_t *sem);
// sem_wait的非阻塞版本
int sem_trywait(sem_t *sem);
// 对信号量进行清理
int sem_destory(sem_t *sem);
```
### 互斥量
基本函数如下
```
#include <pthread.h>

// 初始化互斥量
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mmutexattr);
// 锁定互斥量
int pthread_mutex_lock(pthread_mutex_t *mutex);
// 解锁互斥量
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// 销毁互斥量
int pthread_mutex_destory(pthread_mutex_t *mutex);

// 对已经加锁了的信号量再次加锁会造成线程阻塞
```
### 线程的属性
线程属性的初始化
```
#include <pthread.h>

// 成功返回0,失败返回错误代码
int pthread_attr_init(pthread_attr_t *attr);
// 属性回收
int pthread_attr_destory(pthread_attr_t *attr);

下面是一些常用的函数
// detachstate,这个属性允许我们对线程进行监听.也就是可以不需要使用pthread_join来监听线程的退出.该值有两个标志.1为PTHREAD_CREATE_JOINABLE(可以监听)和PTHReAD_CREATE_DETACHED(不需要监听)
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int detachstate);

// schedpolicy,控制线程的调度方式.取值可以为SCHED_OTHER,SCHED_RP,SCHED_FIFO.默认为SCHED_OTHER,另外两种调度方式只能用于以超级用户权限运行的进程.都被实时调度的功能.SCHED_RP使用循环(round-robin)调度机制,而SCHED_FIFO使用先进先出策略
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int policy);

// schedparam,这个属性结合schedpolicy属性结合使用.它可以对以SCHEd_OTHER策略运行的线程的调度进行控制.
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);

// inheritschd,这个属性可取两个值,PTHREAD_EXPLICIT_SCHED和PTHREAD_INHERIT_SCHED.默认取值为PTHREAD_INHERIT_SCHED,表示调度由属性明确地设置.PTHREAD_EXPLICIT_SCHED将沿用其创建者所使用的参数.
int pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int inherit);

// 控制一个线程调度的计算方式,只支持一种取值PTHREAD_SCOPE_SYSTEM.
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getscope(const pthread_attr_t *attr, int scope);

// stacksize,这个属性控制线程所创建的栈大小.单位为字节.Linux在实现线程时,默认使用的栈很大,所以这个功能对Linux来说显得有些多余
int pthread_attr_setstacksize(pthread_attr_t *attr, int scope);
int pthread_attr_getstacksize(const pthread_attr_t *attr, int scope);
```
### 取消一个线程
```
#include <pthread.h>

// 取消一个线程
int pthread_cancel(pthread_t thread);
// 设置线程自己的取消状态.第一个值可以为PTHREAD_CANCEL_ENABLE.允许线程接收取消消息.PTHREAD_CANCEL_DISABLE.忽略取消消息.oldstate是指用于获取先前的取消状态.可以传NULL
int pthread_setcancelstate(int state, int *oldstate);
// 设置取消类型.type可以为PTHREAD_CANCEL_ASYNCHRONOUS,接收到取消请求后立即采取行动.PTHREAD_CANCEL_DEFERRED.接收到取消请求后,一直等待知道线程执行如下哈数后才采取行动.pthread_join,pthread_cond_wait,pthread_cond_timeout,pthread_testcancel,sem_wait,sigwait
int pthread_setcanceltype(int type, int *oldtype);
```