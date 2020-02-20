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
	char buf[1024];
	int cli_arry[MAXFD];
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
	int maxi =0;
	if(lfd==-1)
	{
		sys_err("socket error ");
	}
	for(i = 0;i<MAXFD;i++)
	{
		cli_arry[i]=-1;
	}
	cli_arry[0]=lfd;

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
				for(i =1;i<MAXFD;i++)
				{
					if(cli_arry[i]==-1)
					{
						cli_arry[i]=cfd;
						break;

					}


				}
				if(i==MAXFD)
				{printf("too many client \n");exit(1);  }
				if(i >maxi)
				{
					maxi =i ;
				}
				if(cfd>maxfd)
				{
					maxfd =cfd;
				}
				if((--sr)==0)
				{
					continue;
				}

			}
			for(i =1;i<=maxfd+1;i++)
			{
				if(cli_arry[i]<0)
				{
					continue;
				}
				if(FD_ISSET(cli_arry[i],&rset))
				{
					int rr=read(cli_arry[i],buf,sizeof(buf));
					if(rr<0)
					{
						sys_err("read error");
					}
					else if(rr==0)
					{
						printf("duankailianjie\n");

						close(cli_arry[i]);
						FD_CLR(i,&aset);
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
