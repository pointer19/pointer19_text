#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	int ret;
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		sys_err("fork error");
	}
	else if(pid == 0)
	{
		pid=setsid();
		if(pid<0)
		{
			sys_err("setsid error");
		}
		ret=chdir("/home/wxy/myfile");
		if(ret < 0)
		{
			sys_err("chdir error");
		}
		umask(022);
		close(STDIN_FILENO);

		int fd;
		fd=open("/dev/null",O_RDWR);
		if(fd<0)
		{
			sys_err("open error");
		}
		dup2(fd,STDOUT_FILENO);
		dup2(fd,STDERR_FILENO);
		while(1);
	}
	else
	{
		exit(1);
	}

	return 0;
}

