#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char* argv[])
{
	int ret;
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	if(pid>0)
	{
		exit(1);
	}
	else
	{
		pid=setsid();
		if(pid<0)
		{
			perror("setsid error");
			exit(1);
		}
		ret=chdir("/home/mwm/");
		if(ret<0)
		{
			perror("chdir error");
			exit(1);
		}
		umask(022);
		int fd=open("/dev/null",O_RDWR);
		if(fd<0)
		{
			perror("open error");
			exit(1);
		}
		dup2(fd,STDOUT_FILENO);
		dup2(fd,STDERR_FILENO);
		dup2(fd,STDIN_FILENO);
	}
}
