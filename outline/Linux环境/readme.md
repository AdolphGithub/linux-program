### 程序参数的获取
getopt用来获取参数.允许用户以-开头的参数,原型如下
```
#include <unistd.h>

int getopt(int argc, char * const argv[], const char *optstring);
// 调用后,将返回匹配的列.如下
getopt(argc, argv, ":if:lr")   // 将匹配-i -f或-l -r的参数.
```
getopt_long也是用来获取参数的,只不过允许用户以--开头.原型如下
```
#include <getopt.h>

int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
// 调用后同getopt.其中option的结构如下.
struct option {
    const char *name;   // 参数名,例如init,设置的时候就是--init
    int         has_arg;// 设置参数的设置,0,不带参数,1必须填写,2,可选参数
    int        *flag;   // 如果设置,就将val的值写入到该指针指向的地址,同时返回0.
    int         val;    // 为该选项返回的值.
};
```
### 环境变量
putenv和getenv用来设置和获取当前的环境变量,例如
```
getenv("HOME"); // 用来获取shell中$HOME的值.
```
其原型如下
```
#include <stdlib.h>

int putenv(char *string);  // 设置值. 例如putenv("HOME=/home/adolph")
char *getenv(char *string);// 获取环境变量,例如getenv("HOME")
```
在stdlib.h中有**environ,用来获取所有的环境变量.格式如HOME=/home/adolph,原型如下
```
#include <stdlib.h>
extern char **environ;
```
### 日期和时间
在linux提供的库中,有一个time.h的函数库,在这里面声明了,一个time_t的长整型.可以使用函数time来获取,原型如下
```
#include <time.h>

time_t time(time_t *tloc);
```
difftime函数用来比较两个时间的差值,原型如下
```
#include <time.h>

double difftime(time_t time1, time_t time0);
```
gmtime能将一个长整型的time_t格式化成tm结构体,原型如下
```
#include <time.h>

struct tm *gmtime(const time_t *timep);
```
tm结构体如下,
```
struct tm
{
    int tm_sec;    /*秒，正常范围0-59， 但允许至61*/
    int tm_min;    /*分钟，0-59*/
    int tm_hour;   /*小时， 0-23*/
    int tm_mday;   /*日，即一个月中的第几天，1-31*/
    int tm_mon;    /*月， 从一月算起，0-11*/  1+p->tm_mon;
    int tm_year;   /*年， 从1900至今已经多少年*/  1900＋ p->tm_year;
    int tm_wday;   /*星期，一周中的第几天， 从星期日算起，0-6*/
    int tm_yday;   /*从今年1月1日到目前的天数，范围0-365*/
    int tm_isdst;  /*日光节约时间的旗标*/
}
```
把分解出来的tm结构体转化为time_t使用如下函数.
```
#include <time.h>

time_t mktime(struct tm *tm);
```
如果需要格式化输出,可以使用如下的函数.
```
#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <time.h>

char *strptime(const char *s, const char *format, struct tm *tm);
// 例如
char buf[512];
char *date = strptime(buf, "Y-m-d H:i:s", tm);  // 将得到2019-02-10 10:10:10的时间.
```
### 临时文件
创建临时文件可以使用tmpnam和tmpfile,不同的是,tmpnam被多个程序调用时可能会产生相同的临时文件,而tmpfile.两个原型如下
```
#include <stdio.h>

char *tmpnam(char *s);  // 如果传递了s,则会存到s中
FILE *tmpfile(void);    // 直接生成并打开临时文件.
```
### 用户信息
获取用户的信息可以使用如下的函数.例如
```
#include <sys/types.h>
#include <unistd.h>

uid_t getuid(void);     // 获取当前执行用户的uid.
char *getlogin(void);   // 获取当前执行用户的名称.

#include <sys/types.h>
#include <pwd.h>

struct passwd *getpwnam(const char *name);  // 获取当前目录的所属用户名称
struct passwd *getpwuid(uid_t uid);         // 获取当前目录的用户id.
```
passwd结构体如下
```
struct passwd {
    char   *pw_name;       /* 用户名称 */
    char   *pw_passwd;     /* 用户密码 */
    uid_t   pw_uid;        /* 用户id */
    gid_t   pw_gid;        /* 当前用户所属组的id */
    char   *pw_gecos;      /* 用户信息 */
    char   *pw_dir;        /* 当前用户的主目录 */
    char   *pw_shell;      /* 用户的shell */
};
```
如果要批量读取用户信息,则采用如下几个函数
```
#include <sys/types.h>
#include <pwd.h>

struct passwd *getpwent(void);  // 从/etc/passwd中一行一行的读取.
void setpwent(void);            // 重置读取的位置.
void endpwent(void);            // 结束读取.
```
### 主机信息
获取主机的主机名称
```
#include <unistd.h>

int gethostname(char *name, size_t len);        // 获取len长度的主机名称.如果成功返回0,否则返回1
int sethostname(const char *name, size_t len);  // 设置主机名称 
```
如果需要详细的获取主机的信息
```
#include <sys/utsname.h>

int uname(struct utsname *buf);
```
其中,utsname结构体如下
```
struct utsname {
    char sysname[];    /* 系统名称 */
    char nodename[];   /* 主机名 */
    char release[];    /* 系统发行级别 */
    char version[];    /* 系统版本 */
    char machine[];    /* 硬件类型 */
#ifdef _GNU_SOURCE
    char domainname[]; /* NIS or YP域名 */
#endif
};
```
### 日志
```
#include <syslog.h>

void openlog(const char *ident, int option, int facility);  // 打开一个日志.
void syslog(int priority, const char *format, ...);         // 发送日志.
void closelog(void);                                        // 关闭日志.
```
其中的日志级别包含
```
LOG_EMERG      系统紧急日志
LOG_ALERT      高优先级故障
LOG_CRIT       严重错误
LOG_ERR        错误
LOG_WARNING    警告
LOG_NOTICE     需要注意的特殊情况
LOG_INFO       一般信息
LOG_DEBUG      调试信息
```
日志一般会在/var/log/syslog中.
### 进程
在子进程中获取父级id.如下
```
#include <sys/types.h>
#include <unistd.h>

pid_t getpid(void);     // 获取调用进程id.
pid_t getppid(void);    // 获取调用进程的父级id.
```
### 资源限制
```
NAME_MAX    文件名中的最大字符数
CHAR_BIT    char类型值的位数
CHAR_MAX    char类型的最大值
INT_MAX     int类型行的最大值.
```