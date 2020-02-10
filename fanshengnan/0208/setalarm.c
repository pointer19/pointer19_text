#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/time.h>
int salarm(int s)
{
	struct itimerval newvl,oldvl;
	newvl.it_value.tv_sec=s;
	newvl.it_value.tv_usec=10000;
	newvl.it_interval.tv_sec=1;
	newvl.it_interval.tv_usec=0;

	setitimer(ITIMER_REAL,&newvl,&oldvl);
	printf("sec =%d,usec=%d\n",oldvl.it_value.tv_sec,oldvl.it_value.tv_usec);

	return oldvl.it_value.tv_sec;
}

int main()
{
	int sa1,sa2;
	sa1=salarm(5);
	sleep(3);
	sa2=salarm(10);
	printf("sa1=%d,sa2=%d\n",sa1,sa2);
	while(1);
	return 0;
}

