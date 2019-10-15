#include <stdio.h>

int main(int argc, char **argv) 
{
    int arg = 1;

    if(argc <= 1) {
        printf("请填写参数\n");
        return 0;
    }

    for(;arg < argc; arg++) {
        if(argv[arg][0] == '-') {
            printf("option: %s\n", argv[arg] + 1);
        }else{ 
            printf("unknow option: %s\n", argv[arg]);
        }
    }

    return 0;
}