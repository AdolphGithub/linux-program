### man命令
```
1是普通的命令
2是系统调用,如open,write之类的(通过这个，至少可以很方便的查到调用这个函数，需要加什么头文件)
3是库函数,如printf,fread
4是特殊文件,也就是/dev下的各种设备文件
5是指文件的格式,比如passwd, 就会说明这个文件中各个字段的含义
6是给游戏留的,由各个游戏自己定义
7是附件还有一些变量,比如向environ这种全局变量在这里就有说明
8是系统管理用的命令,这些命令只能由root使用,如ifconfig
9是内核例程
```
例如,man 2 open.