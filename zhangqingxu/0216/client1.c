#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 9999
#define SER_ADDR "127.0.0.1"
int main(int argc, char* argv[])
{
	int fd;
	char buf[1024];
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)
	{
		perror("socket error");
		exit(1);
	}
	struct sockaddr_in ser_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	

	inet_pton(AF_INET,SER_ADDR,&ser_addr.sin_addr.s_addr);
	
	int ct=connect(fd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(ct==-1)
	{
		perror("connect error");
		exit(1);
	}
	while(1)
	{
		int rr=read(STDIN_FILENO,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,rr);
		write(fd,buf,rr);
	}
	return 0;
}

