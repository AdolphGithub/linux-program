### linux 重要设计文件
```
/dev/console    系统控制台
/dev/tty        控制终端,如果有一个进程有控制终端的化,那么特殊文件/dev/tty就是这个控制终端
/dev/null       空(null)设备,所有写向这个设备的输出都将被丢弃.
```
### 文件操作符
```
0: 标准输入
1: 标准输出
2: 标准错误
```
用于open函数的文件操作标识符
```
O_APPEND: 把写入数据追加在文件的末尾
O_TRUNC:  把文件长度设置为0,丢弃已有的内容.
O_CREAT: 如果需要,就按参数mode中给出的访问模式创建文件.
O_EXEC:  与O_CREAT一起使用,确保调用者创建出文件
```
访问模式包含如下
```
O_RDONLY    只读模式
O_WRONLY    只写模式
O_RDWR      读写打开
```
当使用open函数是,如果flags为O_CREAT时,必须要声明权限的初始值.初始值如下.
```
S_IRUSR: 读权限, 文件所属用户
S_IWUSR: 写权限, 文件所属用户
S_IXUSR: 执行权限, 文件所属用户
S_IRGRP: 读权限, 文件所属组
S_IWGRP: 写权限, 文件所属组
S_IXGRP: 执行权限, 文件所属组
S_IROTH: 读权限, 其他用户
S_IWOTH: 写权限, 其他用户
S_IXOTH: 执行权限, 其他用户
```
### 其他跟文件相关的函数
#### lseek
lseek系统调用对文件描述符fildes的读写指针进行设置,原型如下
```
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```
其中whence参数用来在指定位置,如下
```
SEEK_SET: offset是一个绝对位置
SEEK_CUR: offset是根据当前位置的一个相对位置
SEEK_END: offset是相对于文件尾的一个相对位置
```
#### fstat
fstat系统调用返回与打开的文件描述符相关的文件的状态信息,该些信息会写入到statbuf结构体中,原型如下
```
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
```
相关函数stat和lstat返回的是通过文件名查找到的状态信息.但如果文件名是一个符号链接(软链接时),lstat返回的是符号本身的信息,而stat返回的是该链接指向的文件信息.struct stat结构体如下
```
struct stat {
    dev_t     st_dev;         /* 保存文件的设备  */
    ino_t     st_ino;         /* 与该文件的inode */
    mode_t    st_mode;        /* 文件权限和文件类型信息 */
    nlink_t   st_nlink;       /* 该文件上的硬链接个数 */
    uid_t     st_uid;         /* 文件所属用户的id*/
    gid_t     st_gid;         /* 文件所属用户的组 */
    dev_t     st_rdev;        /* 设备id*/
    off_t     st_size;        /* 文件大小单位字节 */
    blksize_t st_blksize;     /* 在文件系统I/O中的块大小 */
    blkcnt_t  st_blocks;      /* 分配的512B块数 */
}
```
其中st_mode包含如下信息
```
S_IFMT     0170000   二进制文件
S_IFSOCK   0140000   socket文件
S_IFLNK    0120000   符号链接(软链接)
S_IFREG    0100000   常规文件
S_IFBLK    0060000   块设备
S_IFDIR    0040000   目录
S_IFCHR    0020000   特殊的字符设备
S_IFIFO    0010000   文件是一个FIFO(命名管道)
```
#### dup和dup2
dup和dup2复制文件操作符,原型如下
```
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
```
dup是给一个文件操作符,得到一个新的操作符,dup2是将oldfd复制给newfd.
### 标准I/O库
详情请查看[标准I/O库](https://github.com/AdolphGithub/modern_c/tree/master/chapter/%E8%BE%93%E5%85%A5%E8%BE%93%E5%87%BA)
### 格式化输入和格式化输出
详情请查看[输入输出](https://github.com/AdolphGithub/modern_c/tree/master/chapter/%E8%BE%93%E5%85%A5%E8%BE%93%E5%87%BA)
### 文件和目录维护
chmod 改变文件权限同linux的命令chmod
```
#include <sys/stat.h>

int chmod(const char *pathname, mode_t mode);
```
chown 改变文件所属用户同linux,chown
```
#include <unistd.h>

int chown(const char *pathname, uid_t owner, gid_t group);
```
unlink 删除文件
```
#include <unistd.h>

int unlink(const char *pathname);
```
link 创建一个新链接
```
#include <unistd.h>
int link(const char *oldpath, const char *newpath);
```
symlink 创建一个符号链接
```
#include <unistd.h>

int symlink(const char *target, const char *linkpath);   
```
mkdir 创建一个目录 rmdir 删除一个目录
```
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);

#include <unistd.h>
int rmdir(const char *pathname);
```
chdir 切换当前目录,和getcwd确定当前程序运行的目录
```
#include <unistd.h>
int chdir(const char *path);

char *getcwd(char *buf, size_t size);
```
### 扫描目录
opendir 打开一个目录流
```
#include <sys/types.h>
#include <dirent.h>

DIR *opendir(const char *name);
```
readdir 读取一个目录,目录流的下个目录进入点。
```
#include <dirent.h>

struct dirent *readdir(DIR *dirp);
// 结构dirent 定义如下：
struct dirent {
    ino_t          d_ino;       /* Inode编号 */
    off_t          d_off;       /* 目录文件开头至此目录进入点的位移 */
    unsigned short d_reclen;    /* d_reclen _name 的长度, 不包含NULL 字符 */
    unsigned char  d_type;      /* 所指的文件类型 */
    char           d_name[256]; /* 文件名字 */
};
```
telldir 返回目录流中的当前位置
```
#include <dirent.h>

long telldir(DIR *dirp);
```
seekdir 设置目录流的目录项指针位置
```
#include <dirent.h>
void seekdir(DIR *dirp, long loc);
```
closedir 关闭目录流
```
#include <sys/types.h>
#include <dirent.h>

int closedir(DIR *dirp);
```
### 错误处理
strerror 得到错误代码的信息
```
#include <string.h>

char *strerror(int errnum);
```
perror 打印出系统的错误信息
```
#include <stdio.h>

void perror(const char *s);
```
### /proc文件系统
proc特殊文件系统,用于访问底层的硬件信息,例如/proc/cpuinfo,/proc/meminfo
### fcntl和mmap
```
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* arg */ );
```
mmap系列函数
```
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int msync(void *addr, size_t length, int flags);
int munmap(void *addr, size_t length);
```