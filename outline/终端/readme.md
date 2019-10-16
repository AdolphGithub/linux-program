### 终端的处理模式
标准模式:用户可以在一行内对行内的内容进行删除或退格.非标准模式则对输入有更精细的控制.
### 输出重定项检查
在linux c中如何检测输出或者输入的重定向.使用isatty函数.函数原型如下
```
#include <unistd.h>

int isatty(int fd);
```
### 对终端的处理
由于在linux中,一切皆是以文件的形式存在的,所以,终端也是一个设备文件.位于/dev/tty.改设备始终是指向当前终端或当前的登录会话.如下.
```
int input = fopen("/dev/tty", "r");
int out = fopen("/dev/tty", "w");

fprintf(out, "输出一个字符串");
fgetc(input); // 读取一个字符串.
```
### GTI通用终端接口
GTI通用终端接口所处理的功能
```
行编辑: 是否允许用退格键进行编辑．
缓存:   是立即读取字符，还是等待一段可配置的延迟之后再读取它们
回显:   允许控制字符的回显,例如读取密码时
回车/换行(CR/LF):   定义如何在输入/输出时映射回车/换行符,比如打印\n字符时应该如何处理
线速:   这一功能很少用于PC控制台, 但对调制解调器或通过串行线进行连接的终端就很重要了.
```
### termios结构
通过设置termios中的结构就能对终端接口进行控制.其结构体如下
```
#include <termios.h>

struct termios {
    tcflag_t c_iflag;      /* 输入模式 */
    tcflag_t c_oflag;      /* 输出模式 */
    tcflag_t c_cflag;      /* 控制模式 */
    tcflag_t c_lflag;      /* 本地模式 */
    cc_t     c_cc[NCCS];   /* 特殊字符 */
}
```
可以调用函数tcgetattr来初始化与一个终端对应的termios结构,该函数的原型如下
```
#include <termios.h>
#include <unistd.h>

int tcgetattr(int fd, struct termios *termios_p);
```
如果要重新配置终端,则使用如下函数.
```
#include <termios.h>
#include <unistd.h>

int tcsetattr(int fd, int optional_actions, const struct termios *termios_p); // optional_actions包含三个值
// TCSANOW: 立即对值进行修改
// TCSADRAIN: 等当前对的输出完成后再对值进行修改.
// TCSAFLUSH: 等当前的输出完成后再对值进行修改,但丢弃还未从read调用返回的当前可用的任何输入.
```
### 输入模式
用于termios中的c_iflag;
```
BRKINT: 当在输入行中检测到一个终止状态(链接丢失)时, 产生一个中断
IGNBRK: 忽略输入行中的终止状态
ICRNL:  将接收到的回车符转换为新行符.
INLCR:  将接收到的新行符转换为回车符
IGNPAR: 忽略奇偶校验错误做出标记
INPCK:  对接收到的字符执行奇偶校验
PARMRK: 对奇偶校验错误作出标记
ISTRIP: 将所有接收到的字符裁剪为7比特
IXOF:   对输入启用软件流控
IXON:   对输出启用软件流控
```
如果BRKINT和IGNBRK标志都未被设置,则输入行中的终止状态就被读取为NULL(0x00)字符.
### 输出模式
用于termios中的c_oflag;
```
OPOST: 打开输出处理功能
ONLCR: 将输出中的换行符转换为回车/换行符
OCRNL: 将输出中的回车符转换为新行符
ONOCR: 在0列不输出回车符
OFILL: 发送填充字符以提供延时
OFDEL: 用DEL而不是NULL字符作为填充字符
NLDLY: 新行符延时选择
CRDLY: 回车符延时选择
TABDLY:制表符延时选择
BSDLY: 退格符延时选择
VTDLY: 垂直制表符延时选择
FFDLY: 换页符延时选择
```
### 控制模式
用于termios中的c_cflag;
```
CLOCAL: 忽略所有调制解调器的状态行
CREAD:  启用字符接收器
CS5:    发送或接收字符时使用5比特
CS6:    发送或接收字符时使用6比特
CS7:    发送或接收字符时使用7比特
CS8:    发送或接收字符时使用8比特
CSTOPB: 每个字符使用两个停止位而不是一个
HUPCL:  关闭时挂断调制解调器
PARENB: 启用奇偶校验码的生成和检测功能
PARODD: 使用奇校验而不是偶校验
```
如果设置了HUPCL标志,当终端驱动程序检测到与终端对应的最后一个文件描述符被关闭时，它将通过设置调制解调器的控制线来挂断电话线路
### 本地模式
用于termios中的c_lflag;
```
ECHO:   启用输入字符的本地回显功能
ECHOE:  接收到ERASE时执行退格,空格,退格的动作组合
ECHOK:  接收到KILL字符时执行行删除操作
ECHONL: 回显新行符
ICANON: 启用标准输入处理
IEXTEN: 启用基于特定实现的函数
ISIG:   启用信号
NOFLSH: 禁止清空队列
TOSTOP: 在试图进行写操作之前给后台进程发送一个信息
```
### 特殊控制字符
用于termios中的c_cc.
```
// 标准模式
VEOF:   EOF字符
VEOL:   EOL字符
VERASE: ERASE字符
VINTR:  INTR字符
VKILL:  KILL字符
VQUIT:  QUIT字符
VSUSP:  SUSP字符
VSTART: START字符
// 非标准模式
VINTR:  INTR字符
VMIN:   MIN值
VQUIT:  QUIT字符
VSUSP:  SUSP字符
VTIME:  TIME值
VSTART: START字符
VSTOP:  STOP字符
```
### 其他函数
```
#include <termios.h>

int tcdrain(int fd);        // 让调用程序一直等待，直到所有排队的输出都已发送完毕
int tcflush(int fd, int queue_selector);    // 用于暂停或重新开始输出
int tcflow(int fd, int action); // 用于清空输入,输出或者两者都清空
```
### 检测键盘操作

