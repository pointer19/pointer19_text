#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/stat.h>
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
	else if(pid>0)
	{
		exit(1);
	}
	else
	{
		pid=setsid();
		if(pid<0)
		{
			perror("sid error");
			exit(1);

		}
		ret=chdir("/home/lxy");
		if(ret<0)
		{
			perror("mulu  error");
            exit(1);

		}
		umask(022);
		close(STDIN_FILENO);
		int fd=open("/dev/null",O_RDWR);
		dup2(fd,STDOUT_FILENO);
		dup2(fd,STDERR_FILENO);

		while(1);
	}



     return 0;
}     

