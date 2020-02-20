#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
int main(int argc,char* argv[])
{
	int fd[2];
	pipe(fd);
	pid_t pid=fork();
	if(pid<0)
	{
		perror("fork error\n");
		exit(1);	
	}
	else if(pid==0)
	{
		close(fd[0]);
		int i;
		for(i=0;i<10;i++)
		{
			dprintf(fd[i],"----%d----\n",i);
			sleep(1);	
		}
	}
	else
	{
		close(fd[1]);
		int epfd=epoll_create(16);
		struct epoll_event exp,exps[10];		
		exp.events=EPOLLIN|EPOLLET;
		exp.data.fd=fd[0];

		fcntl(fd[0],F_SETFL,O_NONBLOCK);//改成非阻塞状态读

		epoll_ctl(epfd,EPOLL_CTL_ADD,fd[0],&exp);
		int ret;
		int rr;
		char buf[32];

		while(1)
	    {
			ret=epoll_wait(epfd,exps,10,-1);
			if(ret<0)
			{
				perror("fork error\n");
			    exit(1);
			}
			else
			{
				while(1)//循环读数据
				{
					rr=read(exps[0].data.fd,buf,4096);
					if(rr<0)
					{
						if(errno==EAGAIN)//以非阻塞读尽数据，会设EAGAIN，就要跳出读循环
						{
							break;	
						}
				    	  printf("fork error\n");
				    	  exit(1);
					}
					else if (rr==0)
					{
						printf("客户端断开链接\n");
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

