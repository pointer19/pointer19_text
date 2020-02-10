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
		newtime.it_value.tv_sec=1;
		newtime.it_value.tv_usec=0;
		newtime.it_interval.tv_sec=0;
		newtime.it_interval.tv_usec=0;
		setitimer(ITIMER_REAL,&newtime,&oldtime);
		int i=0;
		while(1)
		{
       printf("i=%d\n",i++);
    }
		return 0;
}
