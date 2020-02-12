#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>

int main(int agrc,char* agrv[])
{
    int ret,fd;
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
         perror("setsid error");
				 exit(1);
			}
			ret=chdir("/home/fq/mypointer19_text");
			if(ret<0)
			{
          perror("chdir error");
					exit(1);
			}
			umask(0022);
			close(STDIN_FILENO);
			fd=open("/dev/null",O_RDWR);
			if(fd<0)
			{
         perror("open error");
				 exit(1);
			}
			dup2(fd,STDOUT_FILENO);
			dup2(fd,STDERR_FILENO);
		}
		return 0;
}

