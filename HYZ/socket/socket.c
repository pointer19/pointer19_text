#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 8000
#define SER_ADDR "192.168.202.128"
#define BUF_SIZE 4096

void err(char * str)
{
	perror(str);
	exit(1);
}
int main(int argc,char* argv[])
{
	int lfd,cfd;//lfd监听套接字 cfd通信套接字
	char buf[4096];
	struct sockaddr_in ser_addr,cli_addr,cli_ip;//服务器地址结构

	bzero(&ser_addr,sizeof(ser_addr));//结构体清零 不要写在后面
	bzero(&cli_addr,sizeof(cli_addr));
	
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);//所有网址 本地字节 要转换
	
	lfd=socket(AF_INET,SOCK_STREAM,0);	//创建监听套接字
	
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);
	
	int cli_len=sizeof(cli_addr);
	cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);

	char dst[256];
	printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
	while(1)
	{
		int rr=read(cfd,buf,sizeof(buf));
	
		write(STDOUT_FILENO,buf,rr);		
		write(cfd,buf,rr);		
		
		
	}









	return 0;	
}

