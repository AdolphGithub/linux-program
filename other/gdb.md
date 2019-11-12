### gdb基本使用
在linux中,可以使用gdb来调试自己的程序.在编译时增加-g参数能得到对应的代码.如下.
```
gcc -g -c ./debug.c -o debug1
```
使用gdb如下
```
gdb debug1
```
### 基本命令
```
run         // 运行程序
backtrace   // 栈堆跟踪
print 变量   // 输出变量 $1 $<number>是gdb的命令结果.例如print a.则$1=a.如果在使用print j.则$2 = j;如果要打印出连续的数据项.则可以使用@<number>进行调试.例如:print array[0]@5
list        // 列出程序源代码
break 行号   // 在指定行号设置断点.
continue [n]// 跳过断点,或跳过几次断点
display exp // 每次程序停止时,打印exp表达式的值
commands    // 设置每次在断点时,需要执行的命令.以end结束.
info 命令    // 显示已经设置的命令,例如info display,info break
disable 命令 // 禁用命令.例如 disable display exp.
set variable exp // 在断点处设置变量.exp为表达式.例如 ,set variable n = n + 1; n在程序中有指定的声明.
```