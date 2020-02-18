#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#define SER_PORT 8000//宏定义，端口号
#define BUF_SIZE 4096//宏定义，缓冲区大小
#define MAXFD 1024
int main(int argc,char* argv[])
{
	int lfd,cfd;//套接字所用文件描述符
	char buf[BUF_SIZE];//缓冲区
	struct pollfd fds[MAXFD];
	int i,maxi,pr,rr;
	bzero(fds,sizeof(fds));
	lfd=socket(AF_INET,SOCK_STREAM,0);//创建套接字
	struct sockaddr_in ser_addr,cli_addr;//创建服务器地址和客户端地址
	bzero(&ser_addr,sizeof(ser_addr));//服务器地址初始化
	bzero(&cli_addr,sizeof(cli_addr));//客户端地初始化
	ser_addr.sin_family=AF_INET;//ipv4地址
	ser_addr.sin_port=htons(SER_PORT);//端口号本地转网络
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);//ip号地址转网络，表所有地址全部录入
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//服务器地址与链接套接字绑定
	fds[0].fd=lfd;
	fds[0].events=POLLIN;
	maxi=0;
	for(i=1;i<MAXFD;i++)
	{
		fds[i].fd=-1;
	}
	listen(lfd,128);//最大监控数
	while(1)
	{
		pr=poll(fds,maxi+1,-1);
		if(pr<0)
		{
			perror("poll error");
			exit (0);
		}
		else if(fds[0].revents&POLLIN)
		{
			int cli_len=sizeof(cli_addr);
        		cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);//阻塞监听，创建通讯用套接字
        		char dst[256];
        		printf("client is right|IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));//连接成功后输出客户端ip和端口
			for(i=1;i<MAXFD;i++)
			{
				if(fds[i].fd<0)
				{
					fds[i].fd=cfd;
					fds[i].events=POLLIN;
        		                if(i>maxi)
                       			{
						maxi=i;
                       			}
					break;
				}
			}
			if((--pr)==0)
			{
				continue;
			}
		}
		for(i=1;i<maxi+1;i++)
		{
			if(fds[i].fd==-1)
			{
				continue;
			}
			else if(fds[i].revents&POLLIN)
			{
				rr=read(fds[i].fd,buf,sizeof(buf));
				if(rr<0)
				{
					perror("read error");
				}
				else if(rr==0)
				{
					printf("LINK IS OVER\n");
					close(fds[i].fd);
					fds[i].fd=-1;
				}
				else
				{
         			       write(STDOUT_FILENO,buf,rr);
				}
				if((--pr)==0)
				{
					break;
				}
			}
		}
	}
	return 0;
}
