#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/epoll.h>
#define SER_ADDR "198.168.0.145"
#define BUF_SIZE 4096
#define SER_PORT 8000
#define MAX_FD 1024



int main(int agrc,char* agrv[])
{
    int lfd,cfd;
		//int pr;
		int i=0;
		//pthread_t tid;
		char buf[BUF_SIZE];
		//FD_ZERO(&aset);
    struct sockaddr_in  ser_addr,cli_addr;
		bzero(&ser_addr,sizeof(ser_addr));
		bzero(&cli_addr,sizeof(cli_addr));
		ser_addr.sin_family=AF_INET;
		ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		ser_addr.sin_port=htons(SER_PORT);

    lfd=socket(AF_INET,SOCK_STREAM,0);
		//maxfd=lfd;
		int opt=1;
		setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
		int bd=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
		if(bd!=0)
		{
        perror("bind error");
				exit(1);
		}
		listen(lfd,128);
    
    int cli_len=sizeof(cli_addr);

    struct epoll_event cli;
		struct epoll_event cli_array[MAX_FD];

    int epfd=epoll_create(MAX_FD);
		int count;
    
    cli.data.fd=lfd;
		cli.events=EPOLLIN | EPOLLET;
		 epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);

		while(1)
		{
				count=epoll_wait(epfd,cli_array,MAX_FD,-1);
				if(count<0)
				{
					 perror("epoll_wait error");
					 exit(1);
				}
				else if(count>0)
				{
						for(i=0;i<count;i++)
						{
							 if(cli_array[i].events & EPOLLIN)
							 {
                   if(cli_array[i].data.fd==lfd)
									 {
                       cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);
											 char dst[256];
											 printf("client ip:%s,  port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
											 cli.data.fd=cfd;
											 cli.events=EPOLLIN | EPOLLET;
											 fcntl(cfd,F_SETFL,O_NONBLOCK);
											 epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
									 }
									 else
									 {
											while(1)
											 {
											 		int rr=read(cli_array[i].data.fd,buf,5);
											 		if(rr<0)
											 	  {
                            if(errno == EAGAIN)
														{
															 break;
														}
														perror("read error");
													  exit(1);
											    }
											    else if(rr==0)
											    {
													  printf("client close\n");
													  epoll_ctl(epfd,EPOLL_CTL_DEL,cli_array[i].data.fd,NULL);
												    close(cli_array[i].data.fd);
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

