#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#define IP "127.0.0.1"
#define Port 9000
int main(int argc, char *argv[])
{
	char buf[100];
	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(Port);
	inet_pton(AF_INET, IP, &ser_addr.sin_addr.s_addr);
	
	int scfd = socket(AF_INET, SOCK_STREAM, 0);
	socklen_t ser_addrlen = sizeof(ser_addr);
	int a = connect(scfd, (struct sockaddr*)&ser_addr, ser_addrlen);
	if(a == -1)
	{
		perror("connect error:");
		exit(1);
	}

	while(1)
	{
		int rr = read(STDIN_FILENO, buf, 100);
		write(scfd, buf, rr);
	}



	return 0;
}

