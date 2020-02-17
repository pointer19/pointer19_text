#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.202.128"
#define BUF_SIZE 4096

void err(char * str)
{
	perror(str);
	exit(1);
}
void catch_child(int signu)
{
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("waitpid =%d\n",pid);	
	}
}
int main(int argc,char* argv[])
{
	int lfd,cfd;//lfd监听套接字 cfd通信套接字
	char buf[4096];
	//设置子进程信号捕捉
	struct sigaction act;
	act.sa_handler=catch_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags=SA_RESTART;//如果慢速系统调用被信号打断，再回来时要重新去执行我的慢调
	sigaction(SIGCHLD,&act,NULL);
	//服务器地址结构，清0
	struct sockaddr_in ser_addr,cli_addr,cli_ip;
	bzero(&ser_addr,sizeof(ser_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(8000);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	lfd=socket(AF_INET,SOCK_STREAM,0);
	//端口复用，opt=1表示复用  
	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
    //绑定服务器地址结构，阻塞监听
	int br=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(br!=0)
	{
		perror("bind error");
		exit(1);
	}
	listen(lfd,128);

	int cli_len=sizeof(cli_addr);
	while(1)
{
	cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
	//打印客户端地址结构
	char dst[256];
	printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
	//创建进程
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
		if(rr==0)//若对端关闭，子进程退出
		{
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
 	close(cfd);
	return 0;	
}

