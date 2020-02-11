#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
int main(int argc, char* argv[])
{
	pid_t pid = fork();
	if(pid>0)
	{
		sleep(100);
	}
	else if(pid == 0)
	{
		kill(getppid(),SIGBUS);
	}
	 return 0;
}

