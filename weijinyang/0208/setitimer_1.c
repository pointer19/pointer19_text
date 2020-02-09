#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>

void catch(int signo)
{
    printf("catch signal %d\n", signo);
}

int main(int argc, char* argv[])
{
    struct itimerval newvl, oldvl;
    signal(SIGALRM, catch);
    newvl.it_value.tv_sec = 5;
    newvl.it_value.tv_usec = 10000;
    newvl.it_interval.tv_sec = 2;
    newvl.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &newvl, &oldvl);
    sleep(2);
    setitimer(ITIMER_REAL, &newvl, &oldvl);

    printf("sec = %d, usec = %d\n", oldvl.it_value.tv_sec, oldvl.it_value.tv_usec);
    while(1);
    return 0;
}

