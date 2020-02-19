#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
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

int sr;
char buf[4096]={0};


struct pollfd have[1024];
int maxi;
for(int i=0;i<1024;i++)
{
	have[i].fd=-1;
	have[i].events=POLLIN;
}
have[0].fd=sfd;
maxi=0;

while(1)
{
	sr=poll(have,maxi+1,-1);
	if(sr<0)
	{
		perror("select error");
		continue;	
	}

		if(sr>0)
		{
			
		if(have[0].revents&POLLIN)
		{
			int cli_len=sizeof(cli_addr);
			cfd=accept(sfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);
			printf("have connect\n");
			for(int i=1;i<1024;i++)
			{
				if(have[i].fd==-1)
				{
					have[i].fd=cfd;
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
				if(have[i].fd==-1) continue;
				if(have[i].revents&POLLIN)
				{
					int rr=read(have[i].fd,buf,1024);
					if(rr<0)
					{
					  perror("read error");
					  continue;	
					}
					else if(rr==0)
					{
						printf("break connect\n");
						close(have[i].fd);
						have[i].fd=-1;	
					}
					else
					{
						write(STDOUT_FILENO,buf,rr);
						write(have[i].fd,buf,rr);
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

