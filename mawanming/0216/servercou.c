#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define SER_PORT 8000//宏定义，端口号
#define BUF_SIZE 4096//宏定义，缓冲区大小

void catch_child(int signu)
{
	pid_t wpid;
	while((wpid=waitpid(-1,NULL,WNOHANG)))
	{
		if(wpid<0)
		{
			perror("waitpid error");
			exit(1);
		}
	}
}

int main(int argc,char* argv[])
{
	struct sigaction act;
	act.sa_handler=catch_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags=SA_RESTART;
	sigaction(SIGCHLD,&act,NULL);
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
	listen(lfd,128);//最大监控数
	while(1)
	{
		int cli_len=sizeof(cli_addr);
		cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t*)&cli_len);//阻塞监听，创建通讯用套接字
		char dst[256];
		printf("client is right|IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));//连接成功后输出客户端ip和端口
		int pid=fork();
		if(pid<0)
		{
			perror("fork error");
			exit(1);
		}
		else if(pid==0)
		{
			close(lfd);
			while(1)
			{
		        	int rr=read(cfd,buf,sizeof(buf));
        			write(STDOUT_FILENO,buf,rr);
			}
			close(cfd);
		}
		else
		{
			close(cfd);
			continue;
		}
	}
	return 0;
}
