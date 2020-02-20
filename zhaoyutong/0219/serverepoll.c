#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.109.1"
#define BUF_SIZE 1024
#define MAXFD 1024
void sys_err(char* str)
{
  perror(str);
  exit(1);
}
int main(int argc, char* argv[])
{
        int lfd,cfd;
        char buf[BUF_SIZE];
        struct sockaddr_in ser_addr,cli_addr;
        bzero(&ser_addr,sizeof(ser_addr));
        bzero(&cli_addr,sizeof(cli_addr));
        ser_addr.sin_family=AF_INET;
        ser_addr.sin_port=htons(SER_PORT);
        ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        lfd=socket(AF_INET,SOCK_STREAM,0);
        bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
        listen(lfd,128);
        int cli_len=sizeof(cli_addr);
        struct epoll_event cli,cli_arry[MAXFD];
        int i;
        int epfd=epoll_create(MAXFD);
	int count;
	cli.events=EPOLLIN;
        cli.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);
        while(1)
	{
          count=epoll_wait(epfd,cli_arry,MAXFD,-1);
          if(count<0)
          {
            sys_err("epoll_wait error");
	  }
	  else
	  {
 	    for(i=0;i<count;i++)
	       {
		 if(cli_arry[i].data.fd==lfd)
		    {
       		       cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);
                        char dst[256];
                        printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
                        cli.data.fd=cfd;
		        cli.events=EPOLLIN;
			epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
                    }
                  else
	            {    int rr=read(cli_arry[i].data.fd,buf,BUF_SIZE);
			 if(rr<0)
			   {
 				sys_err("read error");
                           }
			 else if(rr==0)
			   {
				epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arry[i].data.fd,NULL);
				close(cli_arry[i].data.fd);
				printf("client close\n");
			   }
			 else
			   {
				write(STDOUT_FILENO,buf,rr);
			   }
			  
 	           }	
	         }
               }
            }
       return 0;
}
