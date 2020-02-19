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

#define SER_PORT 8000
#define SER_ADDR "192.168.182.128"
#define BUF_SIZE 4096

void sys_err(char *str)
{
//printf("huishou");
perror(str);
exit(1);
}
int main(int argc,char * argv[])
{
int lfd,cfd;
char buf[1024];

struct sockaddr_in ser_addr,cli_addr;//cli_ip;
socklen_t cli_addrlen=sizeof(cli_addr);
int sr,maxfd,i;
fd_set rset,aset;

FD_ZERO(&aset);

ser_addr.sin_family =AF_INET;
ser_addr.sin_port =htons(SER_PORT);
ser_addr.sin_addr.s_addr =htonl(INADDR_ANY);

lfd =socket(AF_INET,SOCK_STREAM,0);
FD_SET(lfd,&aset);
maxfd=lfd;
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

while(1)
{
	rset =aset;
	sr =select(maxfd+1,&rset,NULL,NULL,NULL);
	if(sr ==-1)
	{
	sys_err("select error");
	}
	if(sr >0)
	{
		if(FD_ISSET(lfd,&rset))
		{
		cfd =accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_addrlen);
		if(cfd==-1)
		{
		sys_err("accept error");
		}
		char dst[64];
		printf("client is ok  ip:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
		FD_SET(cfd,&aset);
		if(cfd>maxfd)
		{
		maxfd =cfd;
		}
		if((--sr)==0)
		{
		continue;
		}
			
	}
	for(i =lfd +1;i<=maxfd;i++)
	{
		if(FD_ISSET(i,&rset))
		{
		int rr=read(cfd,buf,sizeof(buf));
		if(rr<0)
		{
			sys_err("read error");
		}
		else if(rr==0)
		{
		printf("duankailianjie\n");
		close(i);
		FD_CLR(i,&aset);
		}
		else
		{
		write(STDOUT_FILENO,buf,rr);
		}
		if((--sr)==0)
		{
		break;
		}
		}

	}	
}
}

return 0;

}
