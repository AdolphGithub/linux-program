#!/bin/sh

myvar="Hi there"

echo $myvar
echo "$myvar"
echo '$myvar'
echo \$myvar

echo Enter some text
# 读取输出到变量myvar
read myvar

echo '$myvar' now equals $myvar
exit 0