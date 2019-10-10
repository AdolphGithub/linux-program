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
### shell-变量
通常变量
```
variables="变量1"   如果包含空格,就必须用引号把它们括起来.
```
注意,shell中的双引号能解析变量,单引号不能解析变量.使用$来得到变量值,例如$variables
常用环境变量
```
$HOME   当前用户的家目录
$PATH   以冒号分隔用来搜索命令的目录列表
$PS1    命令提示符,通常用$字符,但在bash中,你可以使用一些更复杂的值
$PS2    二级提示符,用来提示后续的输入,通常是>字符
$IFS    输入域分割符,当shell读取输入时,它给出用来分割单词的一组字符,他们通常是空格,制表符和换行符
$0      shell脚本名字
$#      传递给脚本的参数个数
$$      shell脚本的进程号
$1,$2,...       shell脚本的参数
$*      shell脚本的单个参数
$@      $*的一种精巧变体,它不使用IFS环境变量,所以即使IFS为空,参数也不会挤在一起
```
### shell-条件
格式
```
if [ 条件表达式 ]; then
fi
```
条件表达式可以包含一些字符串比较,算术比较,文件条件测试,如下
```
字符串比较
string1 = string2
string1 != string2
-n string   字符串不为空则为真
-z string   字符串为null则为真
算数比较
expression1 -eq expression2     两个表达式相等则结果为真
expression1 -ne expression2     两个表达式不等则结果为真
expression1 -gt expression2     表达式1>表达式2则为真
expression1 -ge expression2     表达式1>=表达式2则为真
expression1 -lt expression2     表达式1<表达式2则为真
expression1 -le expression2     表达式1<=表达式2则为真
! expression                    表达式结果为假则结果为真
文件条件
-d file     文件是一个目录则为真
-e file     文件存在则结果为真,要注意-e是历史选项,不可移植,用-f代替
-f file     文件是一个通常文件则为真
-g file     文件的set-group-id位被设置则结果为真
-r file     文件可读则为真
-s file     如果文件的大小不为0则结果为真
-u file     文件的set-user-id位被设置则结果为真
-w file     文件可写则为真
-x file     文件可执行则为真
```
### shell-控制结构
判断语句格式
```
if [ 条件表达式 ];then
fi
// 如果有多个
if [ 条件表达式 ];then
elif [　条件表达式2　];then
else
fi
```
循环语句for格式.
```
for variable in values
do
    ....
done
```
循环语句while格式
```
while condition do  // 条件为真就一直执行
    ...
done
```
循环语句until格式
```
until condition // 条件为假则一直执行
do
    ...
done
```
分支语句case,格式如下
```
case variable in
    pattern [ | pattern ] ...) ...;;
    pattern [ | pattern ] ...) ...;;
    ...
esac
```
shell中如果有多个条件表达式,可以用并且(&&)或者或(||)来配合使用
### shell-函数
函数格式如下
```
function_name() {
    ...
}
```
在shell中调用函数直接使用函数就可以了.调用函数参数时,则不需要声明参数名.使用如下规则来获取函数.
```
$#      函数参数个数
$1,$2,...       函数的参数
$*      函数的所有参数
$@      跟$*一致
```
### shell-命令
break命令来跳出当前的循环.
:命令是一个空命令,相当于true的别名,由于是内置命令运行比true快,可读性较差
continue命令,用来跳过当前的一次循环.并进入到下一次循环
.命令用于在当前shell中执行命令,通过创建一个新的环境的子shell,并返回返回码给父shell.
echo命令用来输出字符串　
eval命令用来执行命令.例如 eval "pwd"