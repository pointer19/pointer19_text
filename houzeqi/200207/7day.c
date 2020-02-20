#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
void sys_err(char * str)
{
	perror(str);
	exit(1);
}
int main(int argc, char* agrv[])
{
	pid_t pid;
	int i;
	int pr;
	int fd[2];
	pr=pipe(fd);
	if(pr<0)
	{
		sys_err("pipe error");
	}
	for(i=0; i< 2; i++)
	{
		pid = fork();
		if(pid <0)
		{
			sys_err("fork error");
		}
		else if(pid==0)
		{
			break;
		}
	}
	if(i==0)
	{
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls","-l",NULL);
		close(fd[1]);
		sys_err("execlp error");
	}
	else if(i==1)
	{
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-1",NULL);
		close(fd[0]);
		sys_err("execlp error");
	}
	else
	{
		int wr1=wait(NULL);
		printf("wait1 pid=%d\n",wr1);
		int wr2=wait(NULL);
		printf("wait2 pid=%d\n",wr2);
	}

	return 0;
}

