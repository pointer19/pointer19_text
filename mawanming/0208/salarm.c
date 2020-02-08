#include<stdio.h>
#include <sys/time.h>
#include <unistd.h>
void al(int a)
{
	struct itimerval newvl,oldvl;
	newvl.it_value.tv_sec=a;
	newvl.it_value.tv_usec=0;
	newvl.it_interval.tv_sec=0;
	newvl.it_interval.tv_usec=0;
	setitimer(ITIMER_REAL,&newvl,&oldvl);
}
int main(int argc,char *argv[])
{
	al(2);
	sleep(5);
}
