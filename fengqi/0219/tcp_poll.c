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
#include<sys/select.h>
#include<poll.h>
#define SER_ADDR "198.168.0.145"
#define BUF_SIZE 4096
#define SER_PORT 8000
#define MAX_FD 1024



int main(int agrc,char* agrv[])
{
    int lfd,cfd;
		int pr;
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

    struct pollfd fds[MAX_FD];

		int maxi=0;
		fds[0].fd=lfd;
		fds[0].events=POLLIN;
    for(i=1;i<MAX_FD+1;i++)
		{
				fds[i].fd=-1;
		}

		while(1)
		{
						
						pr=poll(fds,maxi+1,-1);
						if(pr==0)
						{
                perror("select error");
								exit(1);
						}
						else if(fds[0].revents & POLLIN)
						{
	    	          cfd =accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);
				          char dst[256];
				          printf("client is connect IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
									for(i=1;i<MAX_FD;i++)
									{
										if(fds[i].fd<0)
										{
                        fds[i].fd=cfd;
												fds[i].events=POLLIN;
												break;
										}
									}
									if(i==MAX_FD)
									{
                      printf("client full\n");
											exit(1);
									}
									if(i>maxi)
									{
											maxi=i;
									}
									if(--pr==0)
									{
											continue;
									}
						}
						for(i=1;i<maxi+1;i++)
						{
								 if(fds[i].fd==-1)
								 {
										continue;
								 }
								 else if(fds[i].revents & POLLIN)
								 {
										int rr=read(fds[i].fd,buf,1024);
										if(rr<0)
										{
											perror("read error");
											exit(1);
										}
										else if(rr==0)
										{
											 printf("client close\n");
											 close(fds[i].fd);
											 fds[i].fd=-1;
										}
										else
										{
												write(STDOUT_FILENO,buf,rr);
										}
										if(--pr==0)
										{
												 break;
										}
								 }
						}
		}
		return 0;
}

