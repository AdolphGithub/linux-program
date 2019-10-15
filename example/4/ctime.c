#include <time.h>
#include <stdio.h>

int main(void) 
{
    time_t timeval;
    time(&timeval);

    printf("date: %s\n",ctime(&timeval));

    return 0;
}