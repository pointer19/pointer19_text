#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 8886
#define BUFSIZE 1024
#define MAXFD 4096
int main(int argc, char* argv[])     // ET--nonblock
{
	int rr;
	char buf[BUFSIZE];
	int sfd,cfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int cli_len = sizeof(cli_addr);
	bzero(&cli_addr,cli_len);

	int opt = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(sfd,128);


	struct epoll_event ep_e,ep_arry[MAXFD];
	ep_e.events = EPOLLIN | EPOLLET;
	ep_e.data.fd = sfd;
	int epfd = epoll_create(MAXFD);
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ep_e);
	int num;

	while(1)
	{
		num = epoll_wait(epfd,ep_arry,MAXFD,-1);
		if(num<0)
		{
			perror("epoll_wait error");
			exit(1);
		}
		for(int i=0;i<num;i++)
		{
			if(ep_arry[i].events & EPOLLIN)
			{
				if(ep_arry[i].data.fd == sfd)
				{
					cfd = accept(sfd,(struct sockaddr* )&cli_addr,&cli_len);
					char buf1[100];
					printf("client ip = %s , port = %d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,buf1,100),ntohs(cli_addr.sin_port));
					
					fcntl(cfd,F_SETFL,O_NONBLOCK);
					ep_e.data.fd = cfd;
					epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ep_e);
				}
				else
				{
					while(1)
					{
						rr = read(ep_arry[i].data.fd,buf,5);
						if(rr<0)
						{
							if(errno == EAGAIN)
							{
								break;
							}
							perror("read error");
							exit(1);
						}
						else if(rr == 0)
						{
							printf("client close\n");
							close(ep_arry[i].data.fd);
							exit(1);
						}
						else
						{
							write(STDOUT_FILENO,buf,rr);
						}
					}
				}
			}
		}
	}
	return 0;
}

