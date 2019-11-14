#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig) {
    printf("OUCH! - I got signal %d\n", sig);
    (void) signal(SIGINT, SIG_DFL); // 恢复默认设置.
}

int main(void)
{
    (void) signal(SIGINT, ouch);

    while(1) {
        printf("Hello world!\n");
        sleep(1);
    }

    return 0;
}