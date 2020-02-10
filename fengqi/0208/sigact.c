#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>

void catch(int signo)
{
    printf("catch sig %d\n",signo);
		sleep(20);
		printf("sleep 20 s\n");
}
int main(int agrc,char* agrv[])
{
    struct sigaction act,oldact;
		act.sa_handler=catch;
		act.sa_flags=0;
    sigemptyset(&act.sa_mask);
		sigaction(SIGINT,&act,&oldact);
		while(1);
		return 0;
}

