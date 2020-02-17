#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 8886
#define SER_IP "127.0.0.1"
int main(int argc, char* argv[])
{
	int f;
	char buf[1024];
	f = socket(AF_INET,SOCK_STREAM,0);//创建一个套接字
	struct sockaddr_in ser_addr;  //初始化服务器地址结构
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port=htons(SER_PORT);//端口号

	inet_pton(AF_INET,SER_IP,&ser_addr.sin_addr.s_addr);//IP地址

	int cr=connect(f,(struct sockaddr*)&ser_addr,sizeof(ser_addr));//连接
	if(cr==-1)
	{
		perror("connect error");
		exit(1);
	}

	while(1)
	{
		int rr=read(STDIN_FILENO,buf,1024);
		write(STDOUT_FILENO,buf,rr);
		write(f,buf,rr);
	}
	return 0;
}
