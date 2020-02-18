#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define ser_port 9999

int main(int argc, char* argv[])
{
	int fd;
	char buf[100];
	fd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(ser_port);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	bzero(&cli_addr,sizeof(cli_addr));
	bind(fd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(fd,128);
	int i=sizeof(cli_addr);
	int ac=accept(fd,(struct sockaddr*)&cli_addr,&i);
	if(ac==-1)
	{
		perror("accept error");
		exit(1);
	}

	while(1)
	{
		int rr=read(ac,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,rr);
	}


	return 0;
}

