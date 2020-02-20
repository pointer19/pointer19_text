#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<poll.h>
#include<pthread.h>
#define SER_PORT 8000
#define SER_ADDR "127.0.0.1"
#define BUF_SIZE 4096
#define MAXFD 1024

void sys_err(char *str)
{
	//printf("huishou");
	perror(str);
	exit(1);
}
int main(int argc,char * argv[])
{
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;//cli_ip;
	socklen_t cli_addrlen=sizeof(cli_addr);
	ser_addr.sin_family =AF_INET;
	ser_addr.sin_port =htons(SER_PORT);
	ser_addr.sin_addr.s_addr =htonl(INADDR_ANY);
	char buf[BUF_SIZE];
	lfd =socket(AF_INET,SOCK_STREAM,0);
	if(lfd==-1)
	{
		sys_err("socket error ");
	}

	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));
	int fh2=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(fh2==-1)
	{
		(sys_err("bind error"));
	}

	int fh1=listen(lfd,128);
	if(fh1==-1)
	{
		sys_err("listen error");
	}

	struct pollfd fds[MAXFD];
	int maxi,i;
	bzero(fds,sizeof(fds));
	fds[0].fd=lfd;
	fds[0].events=POLLIN;
	for(i = 1;i <MAXFD;i++)
	{
		fds[i].fd=-1;
	}
	maxi =0;
	int pr;
	while(1)
	{
		pr =poll(fds,maxi+1,-1);
		if(pr<0)
		{sys_err("poll error");}
		else if(fds[0].revents&POLLIN)
		{
			cfd =accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
			char dst[64];
			printf("client IP =%s,port =%d,successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
			for(i =1;i<MAXFD;i++)
			{
				if(fds[i].fd<0)
				{fds[i].fd=cfd;
					fds[i].events=POLLIN;
					break;}
			}
			if(i ==MAXFD)
			{printf("too many client \n");
				exit(1);	
			}
			if(i>maxi)
			{
				maxi =i;
			}
			if(--pr ==0)
			{continue;}
		}
		for(i =1;i<=maxi;i++)
		{
			if(fds[i].fd<0)
			{continue;}
			else if(fds[i].revents & POLLIN)
			{
				int rr =read(fds[i].fd,buf,BUF_SIZE);
				if(rr<0)
				{sys_err("read error");	}
				else if(rr ==0)
				{
					printf("duankailianjie\n");
					close(fds[i].fd);
					fds[i].fd=-1;
				}
				else
				{write(STDOUT_FILENO,buf,rr);	}
				if(--pr<=0)
				{
					break;
				}
			}	


		}
	}

	return 0;

}
