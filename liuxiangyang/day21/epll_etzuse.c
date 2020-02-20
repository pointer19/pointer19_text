#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/select.h>
#include<sys/epoll.h>
#define MAXLINE 80
#define SER_PORT 8000

void sys_err(char *str)
{
	perror(str);
	exit(1);
}
int main(int argc, char *argv[])
{
	int lfd,cfd;
	char buf[MAXLINE];
	int ret;	
	struct sockaddr_in ser_addr, cli_addr;
	bzero(&ser_addr,sizeof(ser_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(8000);
	lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1)
	{
		sys_err("create socket error");
	}
	
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	ret = bind(lfd, (struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(ret == -1)
	{
		sys_err("bind error");
	}
	ret = listen(lfd, 128);
	if(ret == -1)
	{
		sys_err("listen error");
	
	}
	struct epoll_event cli,cli_arry[1024];
	int epfd=epoll_create(1024);

	cli.events=EPOLLIN;
	cli.data.fd=lfd;

	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);
	int count;
	int i;
	int cli_len=sizeof(cli_addr);
	while(1)
	{
//		printf("65\n");
		count=epoll_wait(epfd,cli_arry,1024,-1);
		if(count<0)
		{
			sys_err("epoll error");
		}
		else
		{
//			printf("73\n");
			for(i=0;i<count;i++)
			{
				if(cli_arry[i].events&EPOLLIN)
				{
					if(cli_arry[i].data.fd==lfd)
					{
						cfd = accept(lfd, (struct sockaddr*)&cli_addr, (socklen_t *)&cli_len);
						char info[256];
						printf("client is ok IP:%s ,  port:%d\n",inet_ntop(AF_INET,     &cli_addr.sin_addr.s_addr,info,sizeof(info)), ntohs(cli_addr.sin_port));
						cli.data.fd=cfd;
					//	cli.events=EPOLLIN;
						cli.events=EPOLLIN | EPOLLET;
						epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
					}
					else
					{

						int rn = read(cli_arry[i].data.fd, buf, 1);
						if(rn < 0)
						{   
							sys_err("read error");
						}   
						else if(rn == 0)
						{   
							epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arry[i].data.fd,NULL);		

							close(cli_arry[i].data.fd);

							printf("client is miss\n");
						}
						else
						{
							write(STDOUT_FILENO, buf, rn);
														
						}

					}

				}
			}

		}
	}
	return 0;
}
