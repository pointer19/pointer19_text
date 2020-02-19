#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main(int argc,char* argv[])
{
int sfd,cfd;
sfd=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in addr,cli_addr;
addr.sin_family=AF_INET;
addr.sin_port=htons(8000);
addr.sin_addr.s_addr=htonl(INADDR_ANY);
bzero(&cli_addr,sizeof(cli_addr));
bind(sfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
listen(sfd,128);

fd_set sread,aread;
FD_ZERO(&aread);
FD_SET(sfd,&aread);
int maxfd=sfd;
int sr;
char buf[4096]={0};

int bufd[1024];
for(int i=0;i<1024;i++)
{
	bufd[i]=-1;
}
int maxi=0;
bufd[0]=sfd;

while(1)
{
	sread=aread;
	sr=select(maxfd+1,&sread,NULL,NULL,NULL);
	if(sr<0)
	{
		perror("select error");
		continue;	
	}

		if(sr>0)
		{
			
		if(FD_ISSET(sfd,&sread))
		{
			int cli_len=sizeof(cli_addr);
			cfd=accept(sfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);
			FD_SET(cfd,&aread);
			printf("have connect\n");
			if(maxfd<cfd)
			{
				maxfd=cfd;
			}
			for(int i=1;i<1024;i++)
			{
				if(bufd[i]==-1)
				{
					bufd[i]=cfd;
					if(i>maxi)
						maxi=i;
					break;	
				}
			}
			if(!--sr)
			{
				continue;	
			}
		}
			for(int i=1;i<=maxi;i++)
			{
				if(bufd[i]==-1) continue;
				if(FD_ISSET(bufd[i],&sread))
				{
					int rr=read(bufd[i],buf,1024);
					if(rr<0)
					{
					  perror("read error");
					  continue;	
					}
					else if(rr==0)
					{
						printf("break connect\n");
						FD_CLR(bufd[i],&aread);
						bufd[i]=-1;
						close(bufd[i]);	
					}
					else
					{
						write(STDOUT_FILENO,buf,rr);
						write(bufd[i],buf,rr);
					}
					if(!--sr)
					{
						break;
					}
				}
			}
			
		}

		
}
close(sfd);
return 0;
}

