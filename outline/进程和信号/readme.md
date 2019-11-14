### 什么是进程
一个其中运行着一个或多个线程的地址空间和这些线程所需要的系统资源.每个进程会分配一个唯一的数字编号.我们称之为进程标识符或PID.通常的取值范围为2-32768的正整数.
### 进程的常见状态
如何查看进程的常见状态.使用如下命令
```
ps ax
```
其中stat代表了进程当前的状态.状态代码详解.
```
S   睡眠
R   运行.严格来说,应是"可运行",即在可运行队列中,处于正在执行或即将运行状态
D   不可中断的睡眠(等待). 通常是在等待输入或输出完成
T   停止.通常是被shell作业控制所停止,或者进程正处于调试器的控制之下
Z   死(Defunct)进程或僵尸(zombie)进程
N   低优先级任务,nice
W   分页.
s   进程是会话期首进程
+   进程属于前台进程组
l   进程是多线程
<   高优先级任务
```
进程pid为1的是init进程.可以看作操作系统的进程管理器.linux分配进程时用进程调度器来决定下一个时间片应该分配给哪个进程.进程调度器根据nice值来分配调度规则.也可以手动增加权重.如下
```
nice oclock     nice值增加10点
renice pid值    nice值降低10个点
```
如何查看nice值
```
ps -l
```
### 启动新进程
在C中可以使用system函数来创建一个新进程.原型如下
```
#include <stdlib.h>

int system(const char *string); // 执行一个新命令.并且等待执行完成.
```
system函数必须要等到进程结束后才能运行.如果要进行异步则需要将命令改为后台.例如
```
system("ps ax &");
```
system由于这种特性在linux中使用不多.更多的是exec系列函数.如下
```
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ... /* (char  *) NULL */);
int execlp(const char *file, const char *arg, ... /* (char  *) NULL */);
int execle(const char *path, const char *arg, ... /*, (char *) NULL, char * const envp[] */);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
// 以上都是运行一个程序.不同的是,execl,execlp,execle的参数是可变的.execv,execvp,execvpe参数是一个字符串数组.
```
在C中还可以通过fork来创建一个新的进程.这个进程跟当前进程是一样的.不同的是,fork的进程中调用fork函数会返回0.原型如下
```
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);   // fork会返回3个值,一个-1,一个0,一个pid.0为当前为子进程.-1为fork失败.fork失败的原因有很多.通常是子进程数超过了规定的限制(CHILD_MAX).
```
如何等待子进程运行结束?可以使用wait函数.原型如下
```
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *stat_loc); // 返回结果会直接写入stat_loc指针中.
```
stat_loc的值如下
```
WIFEXITED(stat_val)                 // 如果子进程正常结束,它就是一个非0值.
WEXITSTATUS(stat_val)               // 如果WIFEXITED非0,它返回子进程的退出码.
WIFSIGNALED(stat_val)               // 如果子进程因为一个未捕获的信号而终止,它就取一个非0值.
WTERMSIG(stat_val)                  // 如果WIFSIGNALED非0,它返回一个信号代码.
WSTOPSIG(stat_val)                  // 如果WIFSTOPPED非0,它返回一个信号代码.
```
也可以使用waitpid,原型如下
```
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *stat_loc, int options);   // pid为需要等待对的子进程的PID,如果它的值为-1,waitpid将返回任一子进程的信息.options用来改变waitpid的行为.最有用的一项为WNOHAND,作用是防止waitpid调用将调用者执行挂起.
```
周期性检查某个特定的子进程是否已终止,可以使用如下的方式
```
waitpid(child_pid, (int *)0, WNOHANG);
```
### 信号
信号的名称在头文件signal.h定义.如下
```
SIGABORT    *进程异常终止
SIGALRM     超时警告
SIGFPE      *浮点运算异常
SIGHUP      链接挂断
SIGILL      *非法指令
SIGINT      终端中断
SIGKILL     终止进程(此信号不能被捕获或忽略)
SIGPIPE     向无读进程的管道写数据
SIGQUIT     终端退出
SIGSEGV     *无效内存段访问
SIGTERM     终止
SIGUSR1     用户定义信号1
SIGUSR2     用户定义信号2
SIGCHID     子进程已经停止或退出
SIGCONT     继续执行暂停进程
SIGSTOP     停止执行(此信号不能被捕获或忽略)
SIGTSTP     终端挂起
SIGTTIN     后台进程尝试读操作
SIGTTOU     后台进程尝试写操作
```
如何给一个进程发送信号,可以使用kill命令,如下
```
kill -HUP 512   // 给pid为512的进程发送挂断信号.
```
如果给运行某一命令的所有进程发送信号.可以使用killall.如下.
```
killall -HUP inetd  // 给执行inetd程序的进程发送挂断信号.
```
也可以使用signal函数.原型如下
```
#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int);
```
还可以使用kill函数.函数原型如下
```
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);   // 给指定进程发送一个信号.成功返回0.
//除外,还有个延时发送SIGALRM信号的函数.
#include <unistd.h>

unsigned int alarm(unsigned int seconds); // 在seconds秒之后发送一个SIGALRM信号.如果seconds设置为0,则取消所有已设置的请求.
```
在新版本中使用sigaction.函数原型如下
```
#include <signal.h>

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
```
sigaction结构体如下
```
struct sigaction {
    void     (*sa_handler)(int);    // 函数指针.接收到信号时,会将信号传递给该函数.可以设置为SIG_IGN和SIG_DFL.它们分别表示信号将被忽略或把对该信号的处理方式恢复为默认动作.
    void     (*sa_sigaction)(int, siginfo_t *, void *); // 
    sigset_t   sa_mask;             // 信号集,在调用sa_handler所指向的信号处理函数之前,该信号会被加入到进程的信号屏蔽字中
    int        sa_flags;            // 标志位.
    void     (*sa_restorer)(void);
};
```
### 信号集
信号集相关函数
```
#include <signal.h>

int sigaddset(sigset_t *set, int signo);    // 给信号集中添加一个给定信号
int sigemptyset(sigset_t *set);             // 将信号集初始化为空
int sigfillset(sigset_t *set);              // 将信号集初始化包含所有已定义的信号
int sigdelset(sigset_t *set, int signo);    // 从信号集中删除指定信号
int sigismember(sigset_t *set, int signo);  // 判断一个给定信号是不是信号集中的成员
int sigprocmask(int how, const sigset_t *set, sigset_t *oset);  // 根据how指定的形式来修改进程的信号屏蔽字.how可以为SIG_BLOCK,SIG_SETMASK,SIG_UNBLOCK.
int sigpending(sigset_t *set);              // 将被阻塞的信号中停留在待处理状态的一组信号写道参数set指向的信号集中.
int sigsuspend(const sigset_t *sigmask);    // 将进程的屏蔽字替换为由参数sigmask给出的信号集,然后挂起程序的执行.
```