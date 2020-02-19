#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<errno.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<poll.h>

#define MAXFD 1024

void err(char* str)
{
	perror(str);
	exit(1);
}
int main(int argc,char* argv[])
{
	
	int lfd,cfd;
	char buf[4096];
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(8001);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	lfd=socket(AF_INET,SOCK_STREAM,0);
	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(lfd,128);

	struct epoll_event cli,cli_arr[MAXFD];
	int count;
	int i;

	int epfd=epoll_create(MAXFD);
	
	cli.events=EPOLLIN;
	cli.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);

	while(1)
	{
		count=epoll_wait(epfd,cli_arr,MAXFD,-1);
		if(count<0)
			{
				err("epoll_wait error");	
			}
		else
		{
			for(i=0;i<count;i++)
			{
				if(cli_arr[i].events&POLLIN)
				{
					if(cli_arr[i].data.fd==lfd)
					{
						int cli_len=sizeof(cli_addr);
						cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);	
						char dst[256];
						printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
						cli.data.fd=cfd;
						cli.events=EPOLLIN;
						epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);							
					}
					else
					{
						int rr=read(cli_arr[i].data.fd,buf,4096);	
						if(rr<0)
						{
							err("read error\n");
						}
						else if(rr==0)
						{
							epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arr[i].data.fd,NULL);
							close(cli_arr[i].data.fd);
							printf("客户端断开链接\n");
						}
						else
						{
							write(STDOUT_FILENO,buf,rr);	
							
						}
						
					}
				}
			}
		}

	}
	return 0;
}
