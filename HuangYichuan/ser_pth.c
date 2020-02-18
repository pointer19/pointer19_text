#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SER_PORT 8000
#define SER_SIZE 4096
void *tfun(void *arg)
{
	int cfd =(int)arg;
	char buf=buf[BUF_SIZE];
	int rr;
	while(1)
	{
		rr=read(cfd,buf,BUF_SIZE);
		if(rr<0)
		{
			perror("read error");
			exit(1);
		}
		else if(rr==0)
		{
			printf("client out\N");
			break;
		}
		write(STDOUT_FILENO,buf,rr);
	}
	pthread_exit(NULL);
}


int main(int argc,char* argv[])
{
	int lfd,cfd;
	pthread_t tid;
	struct sockaddr_in ser_addr,cli_addr;
	socklen_t cli_len=sizeof(cli_addr);

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port= htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	lfd =socket(AF_INET,SOCK_STREAM,0);
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(lfd,128);
	while(1)
	{
		cfd=accept(lfd, (struct sockaddr*)&cli_addr,cli_len);
		char dst[64];
		printf("client IP:%s,port:%d",inet_ntop(AF_INET,cli_addr.sin_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));
		pthread_create(&tid,NULL,tfun,(void *)cfd);
		pthread_detach(tid);
	}
	return 0;
}

