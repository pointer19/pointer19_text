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
#include<sys/wait.h>

int cfd;

void err(char * str)
{
	perror(str);
	exit(1);
}
void pthread_child(void *arg)
{
	char buf[4096];
	int ir=read(cfd,buf,1024);
	while(ir>0)
	{
		write(STDOUT_FILENO,buf,ir);
		write(cfd,buf,ir);
	}
	close(cfd);
	printf("child close\n");
	return NULL;

}
int main(int argc,char* argv[])
{
	int lfd,cfd;//lfd监听套接字 cfd通信套接字
	char buf[4096];
	pthread_t tid;
	//服务器地址结构，清0
	struct sockaddr_in ser_addr,cli_addr;
	bzero(&ser_addr,sizeof(ser_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(8001);
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
	if(cfd<0)
	{
		perror("accept error");			
	}
	//打印客户端地址结构
	char dst[256];
	printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
	//创建线程
	pthread_create(&tid,NULL,pthread_child,NULL);
	pthread_detach(tid);

}
	close(lfd);	
	return 0;	
}

