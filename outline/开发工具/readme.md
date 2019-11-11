### makefile
用C语言开发过后,管理依赖就成了一个问题.在linux中通常是使用makefile来声明和安装.
#### 依赖关系
makefile文件中要声明依赖关系.如下
```
main.o: main.c a.h  // main.o 依赖于main.c和a.h
2.o:2.c a.h b.h     // 2.o 依赖于2.c a.h b.h
3.o:3.c b.h c.h     // 3.o 依赖于3.c b.h c.h
```
#### 规则
声明依赖关系后,就要创建规则.规则也就是文件的生成命令.例如
```
main.o: main.c a.h
    gcc -c main.c a.h
2.o:2.c a.h b.h
    gcc -c 2.c a.b b.h
3.o:3.c b.h c.h
    gcc -c 3.c b.h c.h
```
#### makefile中的注释
在makefile中使用#来进行注释.在makefile中可以使用宏.例如
```
CC = gcc

myapp: main.o 2.o 3.o
    $(CC) -o myapp main.o 2.o 3.o
```
#### 内置宏
makefile中有几个常用的内置宏.如下
```
$?      当前目标所依赖的文件列表中比当前目标文件还要新的文件
$@      当前目标的文字
$<      当前依赖文件的名字(不含后缀)
$*      不包括后缀名的当前依赖文件的名字
```
#### 常用特殊字符
makefile文件中的常用的特殊字符
```
-: 忽略所有错误
@: 执行这条命令时不要将该命令显示在标准输出上
```
#### 定义新规则
如何自定义一个新的编译规则.例如将.cpp(C++)文件编译成.o文件.如下
```
%.cpp: %o
    $(CC) -xc++ $(CFLAGS) -I$(INCLUDE) -c $<
```
#### 管理函数库
对于大型项目,一种比较方便的做法是用函数库管理多个编译产品.函数库通常是.a结尾的.我们使用makefile来处理这种.
```
.c.a:
    $(CC) -c $(GFLAGS) $<    # $(CC) 代表gcc.使用gcc编译所有的.c文件.输入为.o文件
    $(AR) $(ARFLAGS) %@ %*.o # $(AR) ar,$(ARFLAGS) rv 在使用ar命令将.o文件转为.a文件.
```
#### makfile文件和子目录
如和将makefile文件分离出去.有两个方法.如下
```
# 进入到子目录下执行make命令生成mylib.a函数库文件.
mylib.a:
    (cd mylibdirectory;$(MAKE))  # MAKE为make命令.
```
第二个方法就是添加宏,如下
```
# 编译子目录中的源文件并将目标文件放在该子目录下
.c.o:
    $(CC) $(CFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F) #其中D代表目录.F代表文件名.
# 然后使用如下命令来处理
mylib.a: mydir/2.o mydir/3.o
    ar -rv myliba $?
```
### 源代码控制
推荐使用git就可以了.这里书中讲了4种.如下.
```
SCCS: 源代码控制系统
RCS:  版本控制系统
CVS:  并发版本控制系统
Subversion: svn
```
### 编写手册页
使用groff来处理.命令方面不讲解.
### patch命令
使用patch命令可以对比两个文件中的修改.例如
```
patch 1.txt 2.txt > diffs   // 对比文件1和文件2,并将差异保存在diffs中.
patch 1.txt diffs           // 将1.txt和差异进行合并
patch -R 1.txt diffs        // 还原差异. 
```
### 开发环境
KDevelop是用于C和C++程序的IDE.