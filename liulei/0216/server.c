#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

#define IP "127.0.0.1"
#define Port 8000
int main(int argc, char *argv[])
{
	char buf[100];
	struct sockaddr_in ser_addr,cli_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(Port);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int scfd = socket(AF_INET, SOCK_STREAM, 0);
	int ser_addrlen = sizeof(ser_addr);
	bind(scfd, (struct sockaddr *)&ser_addr, ser_addrlen);
	listen(scfd, 128);
	printf("......");
	socklen_t cliaddr_len = sizeof(cli_addr);
	int acfd;
	acfd = accept(scfd, (struct sockaddr*)&cli_addr, &cliaddr_len);

	while(1)
	{
		int rr = read(acfd, buf, 100);
		write(STDOUT_FILENO, buf, rr);
	}



	return 0;
}

