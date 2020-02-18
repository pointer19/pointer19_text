#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<poll.h>


#define SER_PORT 8000
#define MAXFD 1024
#define BUFSIZE 4096

  
int main(int argc, char* argv[])
  {
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_addrlen = sizeof(cli_addr);	
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	lfd = socket(AF_INET,SOCK_STREAM,0);
	
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
  	
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);
	
	struct pollfd fds[MAXFD];
	int maxi;
	int i;
	int rr;
	char buf[BUFSIZE];
	bzero(fds,sizeof(fds));

	fds[0].fd = lfd;
	fds[0].events = POLLIN;

	maxi = 0;

	for(i = 1;i<MAXFD;i++)
	{
		fds[i].fd = -1;
	}

	int pr;
	while(1)
	{
		pr = poll(fds,maxi+1,-1);
		if(pr < 0)
		{
			perror("poll error");
			exit(1);
		}
		else if(fds[0].revents & POLLIN)
		{
			cfd = accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
			char dst[64];
			printf("client IP : %s PORT : %d successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
			for(i = 1;i < MAXFD; i++)
			{
				if(fds[i].fd < 0)
				{
					fds[i].fd = cfd;
					fds[i].events = POLLIN;
					break;
				}
			}
			if(i == MAXFD)
			{
				printf("too many client\n");
				exit(1);
			}
			if(i > maxi)
			{
				maxi = i;
			}
			if(--pr == 0)
			{
				continue;
			}
		}
		for(i = 1;i < maxi+1;i++)
		{
			if(fds[i].fd == -1)
			{
				continue;
			}
			else if(fds[i].revents & POLLIN)
			{
				rr = read(fds[i].fd,buf,BUFSIZE);
				if(rr < 0)
				{
					perror("read error");
					exit(1);
				}
				else if(rr == 0)
				{
					printf("client disconnected\n");
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else
				{
					write(STDOUT_FILENO,buf,rr);
				}
				if(--pr == 0)
				{
					break;
				}
			}
		}
		
	}
  
	 return 0;
  }
