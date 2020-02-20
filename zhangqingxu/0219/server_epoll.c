#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<sys/socket.h>


#define SER_PORT 8886
#define BUFSIZE 1024
#define MAXFD 100000

int main(int argc, char* argv[])
{
	int sfd,cfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);   //创建套接字

	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	int cli_len=sizeof(cli_addr);
	bzero(&cli_addr,cli_len);     //初始化地址结构

	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	bind(sfd,(struct sockaddr* )&ser_addr,sizeof(ser_addr));    //绑定地址结构
	listen(sfd,128);       //设置监听上限
	char buf[BUFSIZE];
	int rr;

	int epfd=epoll_create(MAXFD);
	struct epoll_event ep_cli,ep_arry[MAXFD];
	ep_cli.events = EPOLLIN;
	ep_cli.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ep_cli);
	int num;
	while(1)
	{
		num = epoll_wait(epfd,ep_arry,1024,-1);
		if(num<0)
		{
			perror("epoll_wait error");
			exit(1);
		}
		for(int i=0;i<num;i++)
		{
			if(ep_arry[i].events & EPOLLIN)
			{
				if(ep_arry[i].data.fd==sfd)
				{
					cfd = accept(sfd,(struct sockaddr* )&cli_addr,&cli_len);
					char buf1[100];
					printf("client ip= %s,port=%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,buf1,100),ntohs(cli_addr.sin_port));
					ep_cli.data.fd = cfd;
					epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ep_cli);
				}
				else
				{
					rr=read(ep_arry[i].data.fd,buf,BUFSIZE);
					if(rr<0)
					{
						perror("read error");
						exit(1);
					}
					if(rr==0)
					{
						printf("client close\n");
						close(ep_arry[i].data.fd);
						epoll_ctl(epfd,EPOLL_CTL_DEL,ep_arry[i].data.fd,NULL);
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

