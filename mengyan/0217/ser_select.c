#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<sys/select.h>


#define SER_PORT 8000
#define SER_IP "192.168.19.144"

void sys_err(char* str)
{
	perror(str);
	exit(1);
}
  
int main(int argc, char* argv[])
  {
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_addrlen = sizeof(cli_addr);
	char buf[1024];
	int maxfd;
	fd_set rset,aset;
	FD_ZERO(&aset);
	
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lfd = socket(AF_INET,SOCK_STREAM,0);
	maxfd = lfd;
	
	FD_SET(lfd,&aset);
	
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
  	
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);
	
	int sr;
	int i = 0;
	int rr;
	while(1)
	{
		rset = aset;
		sr = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr < 0)
		{
			sys_err("select erroe");
		}
		else if(sr > 0)
		{
			if(FD_ISSET(lfd,&rset))
			{
				cfd = accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
				char dst[64];
				printf("client IP : %s PORT : %d successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
				FD_SET(cfd,&aset);
				if(cfd > maxfd)
				{
					maxfd = cfd;
				}
				if(--sr == 0)
				{
					continue;
				}
			}
			for(i = lfd + 1;i < maxfd + 1;i++)
			{
				if(FD_ISSET(i,&rset))
				{
					rr = read(i,buf,sizeof(buf));
					if(rr < 0)
					{
						sys_err("read error");
					}
					else if(rr == 0)
					{
						printf("client lost\n");
						close(i);
					
						FD_CLR(i,&aset);
					}
					else
					{
						
						write(STDOUT_FILENO,buf,rr);
						write(i,buf,rr);
					}
					if(--sr == 0)
					{
						break;
					}
				}
			}
		}
		
	}
  
	 return 0;
  }
