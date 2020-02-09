#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
void catch_sig(int signum)
{
    printf("catch sig %d\n", signum);
    sleep(20);
    printf("sleep 20s\n");
}
int main(int argc, char* argv[])
{
    struct sigaction act, oldact;

    act.sa_handler = catch_sig;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT, &act, &oldact);
    while(1);
    return 0;
}

