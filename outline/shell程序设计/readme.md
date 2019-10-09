### 管道和重定向
```
ls -l > lsoutput.txt    将ls -l的输出保存到文件lsoutput.txt中
ls -l >> lsoutput.txt   将ls -l的输出追加到文件lsoutput.txt中
ls -l >lsoutput.txt 2>outputerror.txt 将ls -l的标准输出保存到lsoutput.txt中,标准错误输出保存到outputerror.txt中
ls -l >/dev/null 2>&1   将ls -l的标准输出保存到/dev/null中,包准错误输出保存到/dev/null中.其中1代表标准输出,2代表标准错误输出,&1代表/dev/null.
```
注意,/dev/null为linux的通用回收站.重定向输入使用<例如
```
more < killcount.txt
```
linux通过管道链接的进程可以同时运行,例如
```
ps | sort > pssort.out  将ps的输出排序后保存到pssort.out中.
```
注意使用管道时,禁止对同一个文件进行写入读取操作.例如
```
cat mydata.txt | sort | uniq > mydata.txt   你将会得到一个空文件.
```
### shell语法