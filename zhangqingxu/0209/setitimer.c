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
	printf("catch signal %d\n",signo);
}
int main(int argc, char* argv[])
{
	struct itimerval newvl,oldvl;
	signal(SIGALRM,catch);
	newvl.it_value.tv_sec=5;
	newvl.it_value.tv_usec=10000; //第一次执行时间
	newvl.it_interval.tv_sec=2;
	newvl.it_interval.tv_usec=0;  //循环执行时间
	setitimer(ITIMER_REAL,&newvl,&oldvl);
	while(1);
	return 0;
}

