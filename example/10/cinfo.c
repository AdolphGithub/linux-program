#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    #ifdef DEBUG
        printf("Compiled: " __DATE__ " at " __TIME__ "\n");
        printf("This is line %d of file %s\n", __LINE__, __FILE__);
    #endif // DEBUG

    printf("hello world");
    return 0;
}