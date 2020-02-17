#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#define SER_PORT 8000

int cfd;
void* pthread(void* argc)
{
	char buf[4096]={0};
	int rr;
	while((rr=read(cfd,buf,1024))>0)
	{
		write(STDOUT_FILENO,buf,rr);
		write(cfd,buf,rr);
	}
	close(cfd);
	printf("child fork socket close\n");
	return NULL;
}
int main(int argc,char* argv[])
{
	int lfd;
	lfd=socket(AF_INET,SOCK_STREAM,0);
	if(lfd<0)
	{
		perror("socket");
		exit(0);
	}
	struct sockaddr_in addr,caddr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(SER_PORT);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&caddr,sizeof(struct sockaddr_in));
	bind(lfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
	listen(lfd,128);
	while(1)
	{	
		int size=sizeof(caddr);
		cfd=accept(lfd,(struct sockaddr*)&caddr,(socklen_t*)&size);	
		if(cfd<0)
		{
			perror("socket");
			continue;
		}
		pthread_t tid;
		pthread_create(&tid,NULL,pthread,NULL);
		pthread_detach(tid);
	}
	close(lfd);
	return 0;
}
