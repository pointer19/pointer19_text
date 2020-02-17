#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>

#define SER_PORT 8886
int main(int argc, char* argv[])
{
	int sfd,cfd;
	char buf[1024];
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	bzero(&cli_addr,sizeof(cli_addr));
	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void* )&opt, sizeof(opt));

	bind(sfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));

	listen(sfd,128);
	int cs=sizeof(cli_addr);

	int sr,maxfd,rr;
	fd_set rset,aset;
	FD_ZERO(&aset);
	maxfd=sfd;
	
	FD_SET(sfd,&aset);
	while(1)
	{
		rset=aset;
		sr=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr<0)
		{
			perror("select error");
			exit(1);
		}
		if(sr>0)
		{
			if(FD_ISSET(sfd,&rset))
			{
				cfd=accept(sfd,(struct sockaddr*)&cli_addr,&cs);
				char buf1[100];
				printf("client ip=%s,port=%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,buf1,100),ntohs(cli_addr.sin_port));
				FD_SET(cfd,&aset);
				if(cfd>maxfd)
				{
					maxfd=cfd;
				}
				if(--sr==0)
				{
					continue;
				}
			}
			for(int i=sfd+1;i<=maxfd;i++)
			{
				if(FD_ISSET(i,&rset))
				{
					rr=read(i,buf,sizeof(buf));
					if(rr<0)
					{
						perror("read error");
						exit(1);
					}
					else if(rr==0)
					{
						printf("client=%d close\n",i);
						FD_CLR(i,&aset);
						close(i);
						continue;
					}
					write(STDOUT_FILENO,buf,rr);
					write(i,buf,rr);
					if(--sr==0)
					{
						break;
					}
				}
			}
		}
	}
	return 0;
}

