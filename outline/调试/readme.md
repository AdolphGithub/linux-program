### 错误类型
```
功能定义错误
设计规划错误
代码编写错误
```
### 常用调试技巧
取样法.即在代码中直接打印出调试信息.例如
```
#ifdef DEBUG
    printf("这里输出一些信息");
#endif
```
然后使用gcc -DDEBUG 选项进行编译即可打印出调试信息
### GDB调试
在linux中,可以使用gdb来调试自己的程序.在编译时增加-g参数能得到对应的代码.如下.
```
gcc -g -c ./debug.c -o debug1
```
使用gdb如下
```
gdb debug1
```
即可以使用gdb来进行调试.
### 其他调试工具
```
lint, 找出程序中的错误代码
ctags,cxref,cflow.
prof/gprof产生执行文档
```
### 断言
使用assert.h函数库.
### 内存调试
electricfence替换为使用计算机处理器虚拟内存.安装如下
```
sudo apt install electric-fence
```
还可以使用valgrind来调试.安装如下.
```
sudo apt install valgrind
```
使用如下.
```
valgrind --leak-check=yes -v ./checker
```