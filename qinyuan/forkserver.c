#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.44.131"
#define BUF_SIZE 4096
void catch_child(int signu)
{
	pid_t wpid;
	while((wpid=waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("wait pid=%d\n",wpid);
	}
}
void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char* argv[])
{
	int lfd,cfd;//lfd监听套接字，cfd通信套接字
	char buf[BUF_SIZE];
	struct sockaddr_in ser_addr,cli_addr;//服务器地址结构，客户端地址结构
	//设置子进程信号捕捉
	struct sigaction act;
	act.sa_handler=catch_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags=SA_RESTART;
	sigaction(SIGCHLD,&act,NULL);
	bzero(&ser_addr,sizeof(ser_addr));//结构体清零
	bzero(&cli_addr,sizeof(cli_addr));
	//地址结构赋值
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	//设置端口复用
	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
	lfd=socket(AF_INET,SOCK_STREAM,0);//创建监听套接字
	//绑定服务器地址结构
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	
	listen(lfd,128);//设置监听上限

	int cli_len=sizeof(cli_addr);
	while(1)
	{
		//阻塞监听
		cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
		char dst[256];
		//打印客户端地址结构
		printf("client successfully IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
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
				if(rr==0)
				{
					close(cfd);
					exit(1);

				}
				write(STDOUT_FILENO,buf,rr);
				write(cfd,buf,rr);
			}
			close(cfd);
		}
	
		else
		{
			close(cfd);
		}
	}
	//处理数据
	close(lfd);
	close(cfd);
	return 0;
}

