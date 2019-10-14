#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <error.h>

// 打开命令.
void printdir(char *dir, int depth);

int main(int argc, char **argv) 
{
    if(argc != 2) {
        printf("请填写正确的命令,例如:./opendir /path/dir\n");
        return 0;
    }

    // 开始打开命令.
    printdir(argv[1], 0);
    printf("done.\n");

    return 0;
}

// 打印一个目录.
void printdir(char *dir, int depth) 
{
    DIR *directory;
    struct dirent *entry;
    // 这里没有进行初始化.
    struct stat info;
    
    if( (directory = opendir(dir)) == NULL ) {
        printf("路径打开失败\n");
        return;
    }
    
    chdir(dir);
    // 开始读取每一列的信息.
    while( (entry =  readdir(directory)) != NULL ) {
        stat(entry->d_name, &info);
        // 如果是一个目录.
        if( S_ISDIR(info.st_mode)) {
            // 跳过当此循环.
            if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            printf("%*s%s/\n", depth, "", entry->d_name);
            printdir(entry->d_name, depth + 1);
        }else{
            printf("%*s%s\n",depth, "", entry->d_name);
        }
    }
    // 改变当前目录.
    chdir("..");
    // 关闭目录流.
    closedir(directory);
}