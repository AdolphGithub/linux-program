#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig) {
    printf("OUCH! - I got signal %d\n", sig);
}

int main(void)
{
    struct sigaction act;

    act.sa_handler = ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act,  0);

    while(1) {
        printf("Hello world!\n");
        sleep(1);
    }

    return 0;
}