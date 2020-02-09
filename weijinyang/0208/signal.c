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
}
int main(int argc, char* argv[])
{
    signal(SIGINT, catch_sig);
    while(1);
    return 0;
}

