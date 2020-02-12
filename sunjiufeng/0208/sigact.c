#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

void catch(int signum)
{
	printf("-----\n");
}

int main(int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = catch;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	while(1);
	return 0;
}

