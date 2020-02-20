#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/socket.h>                                                  
#include<arpa/inet.h>
#include<sys/select.h>
#include<poll.h>
#include<sys/epoll.h> 
#define SER_PORT 8000
#define SER_IP "192.168.11.130"
#define BUFSIZE 4096
#define MAXFD 1024

void sys_err(char * str)
{
	perror(str);
	exit(1);
}



int main()
{
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_addrlen = sizeof(cli_addr);
	
	struct epoll_event exp,exps[MAXFD];
	int epfd ,ecr;
	int maxi;
	int i;
	char buf[MAXFD];
	bzero(buf,sizeof(buf));

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = socket(AF_INET,SOCK_STREAM,0);

	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);
	
	epfd=epoll_create(MAXFD);
	if(epfd==-1)
	{
		sys_err("epoll_create error");
	}

	exp.events=EPOLLIN;
	exp.data.fd=lfd;
	ecr=epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&exp);
	if(ecr==-1)
	{
		sys_err("epoll_ctl1 error");
	}


	while(1)
	{
	
		maxi=epoll_wait(epfd,exps,MAXFD,-1);
		if(maxi==-1)
		{
			sys_err("epoll_wait error");
		}		
		else
		{
			for(i=0;i<maxi;i++)
			{
				if(!(exps[i].events & EPOLLIN))
				{
					continue;
				}
				if(exps[i].data.fd==lfd)
				{
					cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
					char dst[64];
					printf("client IP:%s PORT:%d successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
					exp.data.fd=cfd;
					exp.events=EPOLLIN;
					ecr=epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&exp);
					if(ecr==-1)
					{
						sys_err("epoll_ctl error");
					}
				}
				else
				{
					int rr=read(exps[i].data.fd,buf,1024);
					if(rr==0)
					{
						ecr=epoll_ctl(epfd,EPOLL_CTL_DEL,exps[i].data.fd,NULL);
						if(ecr==-1)
						{
							sys_err("epoll_ctl error");
						}
						close(exps[i].data.fd);
						printf("client closed\n");
					}
					else if(rr>0)
					{
						write(STDOUT_FILENO,buf,rr);
					}
				}
			}
		}
	}
	return 0;
}


