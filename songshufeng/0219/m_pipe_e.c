/*************************************************************************
    > File Name: m_pipe_e.c
    > Created Time: 2020年02月20日 星期四 01时4分10秒
	> File creator: 宋树峰
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
  
int main(int argc, char* argv[])
  {
 	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	} 
	else if(pid == 0)
	{
		close(fd[0]);
		int i;
		for(i = 0;i < 10;i++)
		{
			dprintf(fd[1],"----%d----\n",i);
			sleep(1);
		}
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int epfd = epoll_create(16);
		struct epoll_event exp,exps[10];
		exp.events = EPOLLIN | EPOLLET;
		exp.data.fd = fd[0];

		fcntl(fd[0],F_SETFL,O_NONBLOCK);
		
		epoll_ctl(epfd,EPOLL_CTL_ADD,fd[0],&exp);
		int ret,rr;
		char buf[32];
		
		while(1)
		{
			ret = epoll_wait(epfd,exps,10,-1);
			if(ret < 0)
			{
				perror("epoll_ctl error");
				exit(1);
			}
			else
			{
				rr = read(exps[0].data.fd,buf,5);
				if(rr < 0)
				{
					perror("read error");
					exit(1);
				}
				else if(rr == 0)
				{
					printf("end on closed\n");
					exit(1);
				}
				else
				{
					write(STDOUT_FILENO,buf,rr);
				}
			}
		}
	}
  
	 return 0;
  }



