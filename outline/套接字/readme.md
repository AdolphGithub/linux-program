### 套接字
一种通信机制,可以跨网络进行.通常都是通过套接字来进行通信的.
### 套接字的属性
```
域(domain)      可以为AF_INET(Internet网络).AF_INET6等
类型(type)      分为SOCK_STREAM(二进制流)和SOCK_DGRAM(datagram数据报套接字)
协议(protocol)  
```
### 基本函数
```
#include <sys/types.h>
#include <sys/socket.h>

// 创建套接字.第一个域,类型和协议
int socket(int domain, int type, int protocol);
```
其中domain可以取如下的值
```
AF_UNIX     UNIX域协议(文件系统套接字)
AF_INET     ARPA因特网协议(UNIX网络套接字)
AF_ISO      ISO标准协议
AF_NS       施乐(Xerox)网络系统协议
AF_IPX      Novell IPX协议
AF_APPLETALK    Appletalk DDS
```
对于每个域都有自己的套接字类型
```
// AF_UNIX
struct sockaddr_un {
    sa_family_t sum_family; // AF_UNIX
    char sun_path[];        // pathname
}
// AF_INET
struct sockaddr_in {
    shore int sin_family;           // AF_INET
    unsigned short int sin_port;    // Port number
    struct in_addr     sin_addr;    // Internet address
}

struct in_addr {
    unsigned long int s_addr;
}
```
命名套接字
```
#include <sys/socket.h>
// 绑定套接字.
int bind(int socket, const struct sockaddr *address, size_t address_len);
// 监听套接字.
int listen(int socket, int backlog);
// 接收连接
int accept(int socket, strcut sockaddr *address, size_t *address_len);
// accept会阻塞程序,知道有客户建立连接为止.可以通过对套接字文件描述符设置为O_NONBLOCK标志来改变这一行为
int flags = fcntl(socket, F_GETFL, 0);
fcntl(socket, F_SETFL, O_NONBLOCK | flags);
// 请求连接
int connect(int socket, const struct sockaddr *address, size_t address_len);
// 关闭套接字
int close(int socket);
```