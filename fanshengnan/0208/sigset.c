#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
int main()
{
	sigset_t myset,oldset;
	sigemptyset(&myset);
	sigaddset(&myset,SIGINT);
	sigprocmask(SIG_BLOCK,&myset,&oldset);

	while(1);
	return 0;
}