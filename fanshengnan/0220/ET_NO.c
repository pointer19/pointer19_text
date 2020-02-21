#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
#include<poll.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>

#define SER_PORT 8000
#define MAXFD 1024
#define BUFSIZE 4096

void sys_err(char *str)
{
	perror(str);
	exit(1);
}
int main()
{
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_addrlen=sizeof(cli_addr);
	char buf[BUFSIZE];
	bzero(buf,sizeof(buf));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	lfd=socket(AF_INET,SOCK_STREAM,0);
	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(lfd,128);
	struct epoll_event cli,cli_arry[MAXFD];
	int i;
	int epfd=epoll_create(MAXFD);
	int count;
	cli.events=EPOLLIN;
	cli.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);
	while(1)
	{
		count=epoll_wait(epfd ,cli_arry,MAXFD,-1);  //读取
		if(count<0)
		{
			sys_err("epoll_ctl error");
		}
		else
		{
			for(i=0;i<count;i++)
			{
				if(cli_arry[i].events & EPOLLIN)
				{
					if(cli_arry[i].data.fd==lfd)
					{
						cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_addrlen);
						char dst[64];
						printf("client IP:%s PORT:%d successful connection\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
						
						fcntl(cfd,F_SETFL,O_NONBLOCK);
						cli.data.fd=cfd;
						cli.events=EPOLLIN | EPOLLET;
						epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
					}
					else
					{
						int rr;
						while(1)
                               		 	{
                                        		rr=read(cli_arry[i].data.fd ,buf,2);
                                        		if(rr<0)
                                        		{
								if(errno==EAGAIN)
								{
									printf("fei zuse duqu dao wenjian mowei\n");
									break;
								}
								else
								{
									sys_err("read error");
								}
                                                	}
							else if(rr==0)
							{
								epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arry[i].data.fd,NULL);
								close(cli_arry[i].data.fd);
								printf("duan kai lian jie\n");
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
		
	}
	return 0;
}


