#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>

void get_sig(int signum)
{
	printf("catch sig =%d\n",signum);
}
int main(int argc, char* argv[])
{
	
	signal(SIGINT,get_sig);
	while(1);	
	return 0;
}

