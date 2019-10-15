#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int opt;

    if(argc <= 1) {
        printf("please input arguments\n");
        return 0;
    }

    while((opt = getopt(argc, argv, ":f:ilr")) != -1) {
        switch (opt)
        {
            case 'i':
            case 'l':
            case 'r':
                printf("option: %c\n", opt);
                break;
            case 'f':
                printf("option: %c\n", opt);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknow option: %c\n", opt);
                break;
        }
    }

    for(; optind < argc; optind++) {
        printf("arguments: %s\n", argv[optind]);
    }
 
    return 0;
}