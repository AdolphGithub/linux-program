gcc参数介绍
```
-c 编译文件,但不链接文件
-o 指定输出的文件
-I 引入指定的文件和函数库
-lm 引入/usr/lib/libm.a的函数库即linux下的math.h，其中-l代表libm.a,如果是lfoo则为libfoo.a
-L 增加编译器的动态库的搜索路径
-l 指定某个动态链接库
-MM 生成依赖关系清单.以便可以直接写入到makefile中
-D 定义一个宏,例如-DDEBUG 则定义了一个DEBUG的宏.如果要赋值则为:-DDEBUG=1
```
在linux中,动态链接库一般位于如下几个地址
```
/usr/lib
/lib
/lib64   这个是64位的默认地址
/usr/local/lib
```