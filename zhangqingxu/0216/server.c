#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define net_port 8886

int main(int argc, char* argv[])
{
	int sr;
	char buf[1024];
	sr = socket(AF_INET,SOCK_STREAM,0);//创建套接字	
	if(sr==-1)
	{
		perror("socket error");
		exit(1);
	}
	struct sockaddr_in ser_addr,cli_addr;     //xinxi
	ser_addr.sin_family=AF_INET;     //初始化服务器地址结构
	ser_addr.sin_port=htons(net_port);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	bzero(&cli_addr,sizeof(cli_addr));//初始化客户端地址结构

	bind(sr,(struct sockaddr*)&ser_addr,sizeof(ser_addr));   //绑定地址结构
	listen(sr,10);       //设置最大监听数

	int cli_len=sizeof(cli_addr);	
	int cfd = accept(sr,(struct sockaddr* )&cli_addr,&cli_len);   //监听
	char arry[100];
	printf("client.IP:%s  port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,arry,100),ntohs(ser_addr.sin_port));   //打印客户端地址
	while(1)
	{
		int rr=read(cfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,rr);
	}
	
	return 0;
}

