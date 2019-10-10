#!/bin/bash

# CD管理工具.
menu_choice=""   # 选择的操作
current_cd=""    # 当前的CD
title_file="./data/title.cdb"    # 需要存储CD信息的文件
tracks_file="./data/tracks.cdb"   # 需要存储CD中的曲目的文件
temp_file=""     # 临时存储文件


# 获取返回值.
get_return() {
    echo "Press return \c"
    read x
    return 0
}
# 请确认操作.
get_confirm() {
    echo "请确认操作? \c"
    while true
    do
        read x
        case "$x" in
            [yY][Ee][Ss] | [yY]) return 0 ;;
            [nN][Oo] | [nN]) 
                echo "您取消了"
                return 1;;
            *)  echo "请输入yes或者no";;
        esac
    done
}
# 设置菜单选项.
set_menu_choice() {
    clear
    echo "选项 :-"
    echo 
    echo "    a) 添加一张新CD"
    echo "    f) 查找一张CD"
    echo "    c) 统计出所有的CD数和曲目数"
    if [ "$cdcatnum" != "" ]; then
        echo "    l) 列出CD: $cdtitle所有的曲目"
        echo "    r) 删除CD: $cdtitle"
        echo "    u) 更新CD: $cdtitle中的曲目"
    fi

    echo "    q) 退出"
    echo 
    echo "请输入你的选项"
    read menu_choice
    return 
}

## 写入一个标题
insert_title() {
    echo $* >> $title_file
    return
}

## 写入一个曲目
insert_track() {
    echo $* >> $tracks_file
    return
}

## 添加一个新的曲目在这个CD里面.
add_record_tracks() {
    echo "请在这张CD中输入曲目"
    echo "如果没有任何曲目则输入q退出"
    cdtrack=1
    cdtitle=""

    while [ "$cdtitle" != "q" ]
    do
        echo "曲目,曲目标题\c"
        read tmp
        cdtitle=${tmp%%,*}
        if [ "$tmp" != "$cdtitle" ];then
            echo "抱歉,不允许使用逗号"
            continue
        fi
        if [ -n "$cdtitle" ];then
            if [ "$cdtitle" != "q" ]; then
                insert_track $cdcatnum,$cdtrack,$cdtitle
            fi
        else
            cdtrack=$((cdtrack-1))
        fi
        cdtrack=$((cdtrack+1))
    done
    return 
}

# 输入新CD的标题信息
add_records() {
    echo "请输入目录名称"
    read tmp
    cdcatnum=${tmp%%,*}

    echo "请输入标题\c"
    read tmp
    cdtitle=${tmp%%,*}

    echo "请输入类别"
    read tmp
    cdtype=${tmp%%,*}

    echo "请输入歌手 \c"
    read tmp
    cdac=${tmp%%,*}

    echo "请确认需要添加的信息"
    echo "$cdcatnum $cdtitle $cdtype $cdac"

    if get_confirm ; then
        insert_title $cdcatnum,$cdtitle,$cdtype,$cdac
        add_record_tracks
    else
        remove_records
    fi

    return 
}

find_cd() {
    if [ "$1" = "n" ]; then
        asklist="n"
    else
        asklist="y"
    fi
    cdcatnum=""
    echo "请输入CD的标题"
    read keyword
    if [ "$keyword" = "" ]; then
        return 0
    fi

    grep "$keyword" $title_file > $temp_file

    set $(wc -l $temp_file)
    linesfound=$1

    case "$linesfound" in
        0)  echo "没有找到CD信息"
            get_return
            return 0
            ;;
        1)  ;;
        2)  echo "对不起,不是唯一的CD信息"
            echo "相关信息"
            cat $temp_file
            get_return
            return 0
            ;;
    esac

    IFS=","
    read cdcatnum cdtitle cdtype cdac < $temp_file
    IFS=" "

    if [ -z "$cdcatnum" ];then
        echo "对不起,无法解析对应的数据"
        get_return
        return 0
    fi

    echo 
    echo "序号: $cdcatnum"
    echo "标题: $cdtitle"
    echo "类型: $cdtype"
    echo "作曲家: $cdac"
    echo 
    get_return

    if [ "$asklist" = "y" ];then
        echo "是否要查看所有的曲目?"
        read x
        if [ "$x" = "y" ]; then
            echo 
            list_tracks
            echo
        fi
    fi

    return 
}

update_cd() {
    if [ -z "$cdcatnum" ]; then
        echo "请选择CD"
        find_cd n
    fi

    if [ -n "$cdcatnum" ];then
        echo "当前的曲目:"
        list_tracks

        echo
        echo "是否要输入新的曲目到$cdtitle"
        get_confirm && {
            grep -v "^${cdcatnum}," $tracks_file > $temp_file
            mv $temp_file $tracks_file
            echo
            add_record_tracks
        }
    fi
    return 
}
# 统计cd中的曲目
count_cds() {
    set $(wc -l $title_file)
    num_titles=$1
    set $(wc -l $tracks_file)
    num_tracks=$1
    echo "找到$num_titles cd,有$num_tracks 首曲目"
    get_return
    return 
}
# 删除数据CD
remove_records() {
    if [ -z "$cdcatnum" ];then
        echo "请选择CD"
    fi
    if [ -n "$cdcatnum" ];then
        echo "您是否要删除这张CD"
        get_confirm && {
            grep -v "^${cdcatnum}," $title_file > $temp_file
            mv $temp_file $title_file
            grep -v "^${cdcatnum}," $tracks_file > $temp_file
            mv $temp_file $tracks_file
            cdcatnum=""
            echo "删除成功"
        }
        get_return
    fi
    return
}

list_tracks() {
    if [ "$cdcatnum" = "" ];then
        echo "没有cd选中"
        return
    else
        grep "^${cdcatnum}," $tracks_file > $temp_file
        num_tracks=$(wc -l $temp_file)
        if [ "$num_tracks" = "0" ];then
            echo "在$cdtitle中没有找到任何曲目信息"
            else {
                echo 
                echo "$cdtitle:"
                echo 
                cut -f 2- -d , $temp_file
                echo
            } | ${PAGER:-more}
        fi
    fi
    return
}

# 中断时要删除对应的临时文件.
trap "rm -rf $temp_file" EXIT

if [ ! -f $title_file ];then
    touch $title_file
fi

if [ ! -f $tracks_file ];then
    touch $tracks_file
fi

clear
echo 
echo 
echo "迷你的CD管理"
sleep 1

quit=n
while [ "$quit" != "y" ];
do
    set_menu_choice
    case "$menu_choice" in
        a) add_records;;
        r) remove_records;;
        f) find_cd y;;
        u) update_cd;;
        c) count_cds;;
        l) list_tracks;;
        b)
            echo
            more $title_file
            echo 
            get_return;;
        [qQ]) quit = "y";;
        *) echo "请选择正确的菜单";;
    esac
done
exit 0