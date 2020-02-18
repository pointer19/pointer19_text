#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>


#define net_port 8000

void get_sig(int signo)
{
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("wait child = %d\n",pid);
	}
}


int main(int argc, char* argv[])
{
	int sfd;
	char buf[1024];

	struct sigaction act;
	act.sa_handler=get_sig;
	act.sa_flags=SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD,&act,NULL);
	sfd = socket(AF_INET,SOCK_STREAM,0);//创建套接字	
	if(sfd==-1)
	{
		perror("socket error");
		exit(1);
	}
	//设置端口复用
	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void* )&opt, sizeof(opt));
	struct sockaddr_in ser_addr,cli_addr;     //xinxi
	ser_addr.sin_family=AF_INET;     //初始化服务器地址结构
	ser_addr.sin_port=htons(net_port);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	bzero(&cli_addr,sizeof(cli_addr));//初始化客户端地址结构

	int b=bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));   //绑定地址结构
	if(b==-1)
	{
		perror("bind error");
		exit(1);
	}
	listen(sfd,128);       //设置最大监听数
	while(1)
	{
		int cli_len=sizeof(cli_addr);	
		int cfd = accept(sfd,(struct sockaddr* )&cli_addr,&cli_len);   //监听
		if(cfd==-1)
		{
			perror("accept error");
			exit(1);
		}
		char arry[100];
		printf("client.IP:%s  port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,arry,100),ntohs(cli_addr.sin_port));   //打印客户端地址
		pid_t pid=fork();
		if(pid<0)
		{
			perror("fork error");
			exit(1);
		}
		else if(pid==0)
		{
			close(sfd);
			while(1)
			{
				int rr=read(cfd,buf,sizeof(buf));
				if(rr==0)
				{
					exit(1);
				}
				write(STDOUT_FILENO,buf,rr);
			}
			close(cfd);
		}
		else 
		{
			close(cfd);
			continue;
		}
		close(sfd);
	}
	return 0;
}

