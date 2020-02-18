#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#define SER_PORT 8000//宏定义，端口号
#define BUF_SIZE 4096//宏定义，缓冲区大小
int main(int argc,char* argv[])
{
	int lfd,cfd;//套接字所用文件描述符
	char buf[BUF_SIZE];//缓冲区
	fd_set rset,aset;
	FD_ZERO(&aset);
	int maxfd;
	lfd=socket(AF_INET,SOCK_STREAM,0);//创建套接字
	maxfd=lfd;
	FD_SET(lfd,&aset);
	struct sockaddr_in ser_addr,cli_addr;//创建服务器地址和客户端地址
	bzero(&ser_addr,sizeof(ser_addr));//服务器地址初始化
	bzero(&cli_addr,sizeof(cli_addr));//客户端地初始化
	ser_addr.sin_family=AF_INET;//ipv4地址
	ser_addr.sin_port=htons(SER_PORT);//端口号本地转网络
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);//ip号地址转网络，表所有地址全部录入
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//服务器地址与链接套接字绑定
	listen(lfd,128);//最大监控数
	int sr,i,rr;
	while(1)
	{
		rset=aset;
		sr=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr<0)
		{
			perror("select error");
			exit(1);
		}
		else if(sr>0)
		{
			if(FD_ISSET(lfd,&rset))
			{
				int cli_len=sizeof(cli_addr);
			        cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);//阻塞监听，创建通讯用套接字
			        char dst[256];
			        printf("client is right|IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));//连接成功后输出客户端ip和端口
				FD_SET(cfd,&aset);
				if(cfd>maxfd)
				{
					maxfd=cfd;
				}
				if((--sr)==0)
				{
					continue;
				}
			}
			for(i=lfd+1;i<maxfd+1;i++)
			{
				if(FD_ISSET(i,&rset))
				{
					rr=read(cfd,buf,sizeof(buf));
					if(rr<0)
					{
						perror("read error");
						exit(1);
					}
					else if(rr==0)
					{
						printf("LINK IS OVER\n");
						close(i);
						FD_CLR(i,&aset);
					}
					else
					{
           					write(STDOUT_FILENO,buf,rr);
					}
					if((--sr)==0)
					{
						break;
					}
				}
			}
		}
	}
	return 0;
}
