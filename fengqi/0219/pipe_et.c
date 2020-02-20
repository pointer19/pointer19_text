#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<fcntl.h>

int main(int agrc,char* agrv[])
{
    int fd[2];
		pipe(fd);
		pid_t pid;
		int buf[10];
		int i=0;
		pid=fork();
    if(pid<0)
		{
			perror("fork error");
			exit(1);
		}
		if(pid==0)
		{
			close(fd[0]);
			for(i=0;i<10;i++)
			{
				dprintf(fd[1],"----%d----\n",i);
				sleep(3);
			}
			close(fd[1]);
		}
		else if(pid>0)
		{
       struct epoll_event epx,epxs[10];
			 int epfd=epoll_create(20);
			 int count=0;
			 epx.data.fd=fd[0];
			 fcntl(fd[0],F_SETFL,O_NONBLOCK);
			 epx.events=EPOLLIN | EPOLLET;
			 epoll_ctl(epfd,EPOLL_CTL_ADD,fd[0],&epx);
			 while(1)
			 {
						count=epoll_wait(epfd,epxs,10,-1);
						if(count<0)
						{
                perror("epoll_wait error");
								exit(1);
						}
						else 
						{
							 while(1)
							 {
							   int rr=read(epxs[0].data.fd,buf,5);
							   if(rr<0)
							   {
									 if(errno==EAGAIN)
									 {    
												break;
												perror("read error");
												exit(1);
									 }
							   }
							   else if(rr==0)
							   {
								 	 printf("oppsite close\n");
									 exit(1);
							   }
							   else
							   {
								   	write(STDOUT_FILENO,buf,rr);
							   }
							 }
						}
			 }
		}
		return 0;
}

