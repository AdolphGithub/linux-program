#!/bin/sh

echo "Is it morning? Please answer yes or no"
read timeofday

# 为什么要加引号,则是为了如果直接输入enter,则timeofday为空,则条件表表达式则成了if [ = "yes" ]
if [ "$timeofday" = "yes" ];then
    echo "Good morning"
elif [ "$timeofday" = "no" ];then
    echo "Good afternoon"
else
    echo "Sorry, $timeofday not recognized.Enter yes or no"
    exit 1
fi

exit 0