#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/time.h>
#include<signal.h>

void catch(int signo)
{
    printf("catch %d\n",signo);
}
int main(int agrc,char* agrv[])
{
    struct itimerval newtime,oldtime;
		newtime.it_value.tv_sec=10;
		newtime.it_value.tv_usec=0;
		newtime.it_interval.tv_sec=2;
		newtime.it_interval.tv_usec=0;
		signal(SIGALRM,catch);
		setitimer(ITIMER_REAL,&newtime,&oldtime);
		printf("sec=%d,usec=%d\n",oldtime.it_value.tv_sec,oldtime.it_value.tv_usec);
		while(1);
		return 0;
}
