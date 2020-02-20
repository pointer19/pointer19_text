#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#define BUF_SIZE 4096
#define SER_PORT 8000//宏定义端口号
#define SER_ADDR "192.168.159.128"//宏定义服务器地址
int main(int argc,char *argv[])
{
	int cfd;
	char buf[BUF_SIZE];
	cfd=socket(AF_INET,SOCK_STREAM,0);//创建通讯套接字
	struct sockaddr_in ser_addr;//创建服务器地址
	int dst;
	inet_pton(AF_INET,SER_ADDR,&dst);
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=dst;
	connect(cfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	while(1)
	{
		int rr=read(STDIN_FILENO,buf,sizeof(buf));
		write(cfd,buf,rr);
	}
	return 0;
}
