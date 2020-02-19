#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/poll.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.120.1"
#define BUF_SIZE 1024
#define MAXFD 1024

void sys_err(char* str)
{
	perror(str);
	exit(1);
}
int main(int argc, char* argv[])
{     
	int i,maxi;
	int lfd,cfd;
	char buf[BUF_SIZE];
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	lfd=socket(AF_INET,SOCK_STREAM,0);
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(lfd,128);
	int cli_len=sizeof(cli_addr);
	struct pollfd fds[MAXFD];
	bzero(fds,sizeof(fds));
	fds[0].fd=lfd;
	fds[0].events=POLLIN;
	for(i=1;i<MAXFD;i++)
	{
		fds[i].fd=-1;
	}
	maxi=0;
	int pr;
	while(1)
	{
		pr= poll(fds,maxi+1,-1);
		if(pr<0)
		{
			perror("poll error");
			exit(1);
		}
		else if(fds[0].revents&POLLIN)
		{
			cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);
			char dst[256];
			printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
			for(i=1;i<MAXFD;i++)
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
				printf("too many client\n");
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
		for(i=1;i<maxi+1;i++)
		{
			if(fds[i].fd==-1)
			{
				continue;
			}
			else if(fds[i].revents & POLLIN)
			{
				int rr= read(fds[i].fd,buf,BUF_SIZE);
				if(rr<0)
				{ 
					perror("read error");
				}
				else if(rr==0)
				{
					printf("client lost");
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
