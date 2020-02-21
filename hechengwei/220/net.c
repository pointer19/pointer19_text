#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<poll.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<fcntl.h>

#define SER_PORT 8000
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
bzero(buf,sizeof(buf));
int opt=1;
int i;
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
int count;
struct epoll_event cli,cli_arry[MAXFD];
int epfd =epoll_create(MAXFD);
cli.events=EPOLLIN||EPOLLET;
cli.data.fd =lfd;
epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);
while(1)
{	
	count =	epoll_wait(epfd,cli_arry,MAXFD,-1);
	if(count<0)
	{sys_err("poll error");}
	else 
	{
		for(i =0;i<count;i++)
		{
		if(cli_arry[i].events&EPOLLIN)
		{
			if(cli_arry[i].data.fd ==lfd)
			{cfd =accept(lfd,(struct sockaddr *)&cli_addr,&cli_addrlen);	
			char dst[64];
			printf("client IP:%s,PORT :%dsuccessful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
		fcntl(cfd,F_SETFL,O_NONBLOCK);
		cli.data.fd =cfd;
		cli.events =EPOLLIN;		
			epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
			}
			else
			{
		while(1){
		int rr =read(cli_arry[i].data.fd,buf,2);
			if(rr <0)
			{
			if(errno ==EAGAIN)
			{
			printf("dsomowei\n");break;
			}else{
		sys_err("read error");
	}
				}
			else if(rr==0)
			{epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arry[i].data.fd,NULL);
		close(cli_arry[i].data.fd);
		printf("duankailianjie\n");	 
	break;}
		else
		{
		write(STDOUT_FILENO,buf,rr);
		}
	}

	}
	}

	}
}
}
return 0;

}
