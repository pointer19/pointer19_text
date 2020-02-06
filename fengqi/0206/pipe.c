#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>


int main(int agrc,char* agrv[])
{
    int fd[2];
		char buf[1024];
		int pr=pipe(fd);
		int fd1;
		if(pr<0)
		{
			perror("pipe error");
			exit(1);
		}
		pid_t pid=fork();
		if(pid==0)
		{
       sleep(1);
			 close(fd[0]);
			 write(fd[1],"./2.txt",7);
			 close(fd[1]);
			 fd1=open("./2.txt",O_RDWR | O_CREAT,0644);
			 printf("fd1=%d",fd1);
			 dup2(fd1,STDOUT_FILENO);
			 execlp("ls","ls","-l",NULL);
		}
		if(pid>0)
		{
      close(fd[1]);
			int rr=read(fd[0],buf,7);
			close(fd[0]);
			execlp("wc","wc","-l",buf,NULL);
		}
		return 0;
}

