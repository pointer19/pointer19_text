#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
int main(int argc, char* argv[])
{
    struct itimerval newvl, oldvl;
    newvl.it_value.tv_sec = 5;
    newvl.it_value.tv_usec = 10000;
    newvl.it_interval.tv_sec = 2;
    newvl.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &newvl, &oldvl);
    
    while(1);
    return 0;
}

