#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#define MAXFD 1024

int main(int argc, char* argv[])
{
	int fd[2];
	pipe(fd);
	pid_t pid;
	pid = fork();
	int rr;
	char buf[1024];
	fcntl(fd[0],F_SETFL,O_NONBLOCK);
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		close(fd[0]);
		for(int i = 0; i<10;i++)
		{
			dprintf(fd[1],"----%d----\n",i);
			sleep(2);
		}
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		struct epoll_event ep_e,ep_arry[MAXFD];
		ep_e.events = EPOLLIN | EPOLLET;
		ep_e.data.fd = fd[0];
		int epfd = epoll_create(MAXFD);
		epoll_ctl(epfd,EPOLL_CTL_ADD,fd[0],&ep_e);

		while(1)
		{
			int num = epoll_wait(epfd,ep_arry,MAXFD,-1);
			if(num<0)
			{
				perror("epoll_wait error");
				exit(1);
			}
			else
			{
				while(1)
				{
					rr = read(ep_arry[0].data.fd,buf,5);
					if(rr<0)
					{
						if(errno==EAGAIN)
						{
							break;
						}
						perror("read error");
						exit(1);
					}
					else if(rr==0)
					{
						printf("fd close\n");
						close(ep_arry[0].data.fd);
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

