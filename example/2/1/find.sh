#!/bin/sh

# 查找带有find的文件

for file in *
do
    if grep -q find $file
    then
        echo $file
    fi
done

exit 0