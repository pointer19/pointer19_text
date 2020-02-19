#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/socket.h>                                                  
#include<arpa/inet.h>
#include<sys/select.h>
 
#define SER_PORT 8000
#define SER_IP "192.168.11.130"
#define BUF_SIZE 4096
#define MAXFD 1024

void sys_err(char * str)
{
	perror(str);
	exit(1);
}



int main()
{
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_addrlen = sizeof(cli_addr);
	int sr;
	fd_set rset,aset;
	int maxfd;
	int i;
	char buf[1024];

	FD_ZERO(&aset);

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = socket(AF_INET,SOCK_STREAM,0);

	FD_SET(lfd,&aset);
	maxfd = lfd;

	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);
	
	int cli_arry[MAXFD];
	int maxi = 0;
	for(i=0;i<MAXFD;i++)
	{
		cli_arry[i] = -1;
	}
	
	cli_arry[0]=lfd;
	maxi = 1;

	while(1)
	{
		rset = aset;
		sr = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr==-1)
		{
			sys_err("select error");
		}
		if(sr>0)
		{
			if(FD_ISSET(lfd,&rset))
			{
				cfd = accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
				char dst[64];
				printf("client IP : %s   PORT : %d successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
				FD_SET(cfd,&aset);
				
				for(i = 1;i<MAXFD;i++)
				{
					if(cli_arry[i] == -1)
					{
						cli_arry[i] = cfd;
						break;
					}
				}
				if(i == MAXFD)
				{
					printf("too many client\n");
					exit(i);
				}

				if(i>maxi)
				{
					maxi = i;
				}


				if(cfd > maxfd)
				{
					maxfd = cfd;
				}
				if((--sr) == 0)
				{
					continue;
				}
			}
			for(i=1;i<=maxfd;i++)
			{
				if(cli_arry[i]<0)
				{
					continue;
				}
				else if(FD_ISSET(cli_arry[i],&rset))
				{
					int rr = read(cli_arry[i],buf,1024);
					if(rr<0)
					{
						sys_err("read error");
					}
					else if(rr == 0)
					{
						printf("客户端已断开链接\n");
						close(cli_arry[i]);
						FD_CLR(cli_arry[i],&aset);
						cli_arry[i]=-1;
					}
					else
					{
						write(STDOUT_FILENO,buf,rr);
						write(cli_arry[i],buf,rr);
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


