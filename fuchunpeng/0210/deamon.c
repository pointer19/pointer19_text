/*******************************************************************************************************************************************************************************************************************************************************Mr.Fu**************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
void sys_err(char* str)
{
	perror(str);
	exit(1);
}
		    
int main(int argc, char* argv[])
{
	int ret;
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		sys_err("fork wrong!");
	}
	if(pid>0)
	{
		exit(1);
	}
	else if(pid==0)
	{
		pid=setsid();
		if(pid<0)
		{
			sys_err("setsid wrong!");
		}
		ret=chdir("/home/fcp/point19");
		if(ret<0)
		{
			sys_err("chdir wrong!");
		}
	umask(022);
	close(STDIN_FILENO);
	int fd=open("/dev/null",O_RDWR);
	if(fd<0)
	{
		sys_err("open error!");
	}
	dup2(fd,STDOUT_FILENO);
	dup2(fd,STDERR_FILENO);
	while(1);
	}
return 0;
}

