

#include<stdio.h>
#include<string.h>
#include<error.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>


#define SER_PORT 8000
#define SER_ADDR "192.168.11.130"
#define BUF_SIZE 4096
void sys_err(char * str)
{
	perror(str);
	exit(1);
}
int main(int argc, char* argv[])
{
	
	int lfd,cfd;
	char buf[BUF_SIZE];
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family =AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htons(INADDR_ANY);
	lfd = socket(AF_INET,SOCK_STREAM,0);
	int br=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(br<0)
	{
		sys_err("bind error");
	}
	int lr=listen(lfd,128);
	if(lr<0)
	{
		sys_err("listen error");
	}
	int cli_len = sizeof(cli_len);
	cfd = accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
	if(cfd<0)
	{
		sys_err("accept error");
	}
	char dst[256];
	printf("client is creat connect, IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
	
	while(1)
	{
		int rr = read(cfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,rr);
	}
	return 0;
}


