#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define SER_PORT  8000   //服务器的端口号
#define SER_ADDR "127.0.0.1" //服务器的IP地址
#define BUF_SIZE 4096
int main()
{
	int cfd;
	char buf [BUF_SIZE] ;
	cfd = socket (AF_INET,SOCK_STREAM,0) ;  //客户端套接字
	struct sockaddr_in ser_addr;
	bzero(&ser_addr,sizeof (ser_addr)) ;
	 //初始化服务器的地址结构
	ser_addr.sin_family = AF_INET ;
	ser_addr.sin_port=htons(SER_PORT) ;
	int dst;
	
	inet_pton(AF_INET,SER_ADDR,&ser_addr.sin_addr.s_addr) ;
	
	int cr = connect(cfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));            //和服务器建立连接
	if(cr!=0)
	{
		perror("connect error") ;
		exit(1) ;
	}
	while(1)
	{
		int rr=read(STDIN_FILENO,buf,sizeof(buf)) ;
		write(STDOUT_FILENO,buf,rr) ;
		write(cfd,buf,rr) ;
	}
	return 0;
}

