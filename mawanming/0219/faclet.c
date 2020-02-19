#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
int main(int argc,char* argv[])
{
	int pipefd[2];
	pipe(pipefd);
	fcntl(pipefd[0],F_SETFL,O_NONBLOCK);
	int pid;
	pid=fork();
	if(pid==0)
	{
		close(pipefd[0]);
		for(int i=0;i<10;i++)
		{
			dprintf(pipefd[1],"*****%d***\n",i);
			sleep(3);
		}
	}
	if(pid>0)
	{
		close(pipefd[1]);
		int ep=epoll_create(10);
		struct epoll_event event,eventl[3];
		event.events=EPOLLIN;
		event.data.fd=pipefd[0];
		epoll_ctl(ep,EPOLL_CTL_ADD,pipefd[0],&event);
		int rr;
		char buff[5];
		while(1)
		{
			int ba=epoll_wait(ep,eventl,3,-1);
			if(ba<0)
			{
				perror("epoll_wait error");
			}
			else if(ba>0)
			{
				while(1)
				{
					rr=read(pipefd[0],buff,5);
					if(rr<0)
					{
						if(errno==EAGAIN)
						{
							break;
						}
						else
						{
							perror("read error");
						}
					}
					else if(rr==0)
					{
						printf("CHILD OVER");
					}
					else
					{
						write(STDOUT_FILENO,buff,5);
					}
				}
			}
		}
	}
}
