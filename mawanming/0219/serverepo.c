#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#define SER_PORT 8000//宏定义，端口号
#define BUF_SIZE 4096//宏定义，缓冲区大小
#define MAXFD 1024
int main(int argc,char* argv[])
{
	int epfd=epoll_create(MAXFD);
	struct epoll_event cli,cli_arry[MAXFD];
	int lfd,cfd;//套接字所用文件描述符
	char buf[BUF_SIZE];//缓冲区
	lfd=socket(AF_INET,SOCK_STREAM,0);//创建套接字
	struct sockaddr_in ser_addr,cli_addr;//创建服务器地址和客户端地址
	bzero(&ser_addr,sizeof(ser_addr));//服务器地址初始化
	bzero(&cli_addr,sizeof(cli_addr));//客户端地初始化
	ser_addr.sin_family=AF_INET;//ipv4地址
	ser_addr.sin_port=htons(SER_PORT);//端口号本地转网络
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);//ip号地址转网络，表所有地址全部录入
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//服务器地址与链接套接字绑定
	cli.events=EPOLLIN;
	cli.data.fd=lfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&cli);
	listen(lfd,128);//最大监控数
	int count,i,rr;
	while(1)
	{
		count=epoll_wait(epfd,cli_arry,MAXFD,-1);
		if(count<0)
		{
			perror("epoll_wait error");
		}
		else
		{
			for(i=0;i<count;i++)
			{
				if(cli_arry[i].events&EPOLLIN)
				{
					if(cli_arry[i].data.fd==lfd)
					{
						int cli_len=sizeof(cli_addr);
					        cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);//阻塞监听，创建通讯用套接字
       						char dst[256];
        					printf("client is right|IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));//连接成功后输出客户端ip和端口
						cli.data.fd=cfd;
						cli.events=EPOLLIN;
						epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&cli);
					}
					else
					{
						rr=read(cli_arry[i].data.fd,buf,sizeof(buf));
				                if(rr<0)
						{
							perror("read error");
						}
						else if(rr==0)
						{
							epoll_ctl(epfd,EPOLL_CTL_DEL,cli_arry[i].data.fd,NULL);
							close(cli_arry[i].data.fd);
							printf("LINK IS OVER\n");
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
