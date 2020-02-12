#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/stat.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char* argv[])
{
	pid_t pid = fork();
	if(pid < 0)
	{
		sys_err("fork error");
	}
	else if(pid > 0)
	{
		exit(1);
	}
	else
	{
		pid = setsid();
		if(pid < 0)
		{
			sys_err("setsid error");
		}
		int ret = chdir("/home/sjf/test");
		if(ret < 0)
		{
			sys_err("chdir error");
		}
		umask(022);

		close(STDIN_FILENO);
		int fd  = open("/dev/null", O_RDWR);
		if(fd < 0)
		{
			sys_err("open error");
		}
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
	}
 
	return 0;
}

