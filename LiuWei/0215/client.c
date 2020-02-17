#include<stdio.h>
#include<string.h>
#include<error.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 8000
#define SER_ADDR "127.0.0.1"
#define BUF_SIZE 4096
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char* argv[])
{
	
	int cfd;
	char buf[BUF_SIZE];
	cfd = socket(AF_INET,SOCK_STREAM,0);
	if(cfd<0)
	{
		sys_err("socket error");
	}
	struct sockaddr_in ser_addr;
	bzero(&ser_addr,sizeof(ser_addr));

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);

	int ir=inet_pton(AF_INET,SER_ADDR,&ser_addr.sin_addr.s_addr);
	if(ir<0)
	{
		sys_err("inet_pton error");
	}
	int cr = connect(cfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr));

	if(cr!=0)
	{
		sys_err("connect error");
	}


	while(1)
	{
		int rr = read(STDIN_FILENO,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,rr);
		write(cfd,buf,rr);
	}
	return 0;
}
