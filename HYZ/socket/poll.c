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

	struct pollfd fds[MAXFD];
	int i;
	int maxi;
	int pr;
	int rr;
	bzero(fds,sizeof(fds));
	//初始化数组
	fds[0].fd=lfd;
	fds[0].events=POLLIN;//监听读事件
	//全部初始化为-1,之后找到第一个为-1的值，把cfd放入
	for(i=1;i<MAXFD;i++)
	{
		fds[i].fd=-1;	
	}
	maxi=0;
	while(1)
	{
		pr=poll(fds,maxi+1,-1);	//maxi表示下标，从0开始，表示个数要加一
		if(pr<0)
			{
				err("poll error");	
			}
		else if(fds[0].revents&POLLIN)//POLLIN不一定等于re 但一定包含在re里，返回1,发生
		{
			int cli_len=sizeof(cli_addr);
			cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);	
			char dst[256];
			printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
			for(i=1;i<MAXFD;i++)//把cfd放入数组中，当fd=-1时
			{
				if(fds[i].fd<0)
				{
					fds[i].fd=cfd;
					fds[i].events=POLLIN;
					break;	
				}
			}
			if(i==MAXFD)
			{
				printf("too many\n");
				exit(1);
			}
			if(i>maxi)
			{
				maxi=i;	
			}
			if(--pr==0)
			{
				continue;
			}
		}
		for(i=1;i<maxi+1;i++)//lfd遍历过了，从cfd开始i=1，之后输出
		{
			if(fds[i].fd==-1)
				{
					continue;	
				}
			else if(fds[i].revents&POLLIN)
			{
				rr=read(fds[i].fd,buf,4096);
				if(rr<0)
				{
					err("read error\n");
				}
				else if(rr==0)
				{
					printf("对端关闭\n");
					close(fds[i].fd);	
					fds[i].fd=-1;
				}
				else
				{
					write(STDOUT_FILENO,buf,rr);	
				}
				if(--pr==0)
				{
					break;	
				}

			}
		}
    }
	return 0;	
}

