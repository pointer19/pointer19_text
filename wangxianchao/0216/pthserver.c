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
void* pthr(void* argc)
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
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("socket");
		exit(0);
	}
	struct sockaddr_in addr,caddr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(SER_PORT);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&caddr,sizeof(struct sockaddr_in));
	bind(sfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
	listen(sfd,128);
	while(1)
	{	
		int size=sizeof(caddr);
		cfd=accept(sfd,(struct sockaddr*)&caddr,(socklen_t*)&size);	
		if(cfd<0)
		{
			perror("socket");
			continue;
		}
		pthread_t tid;
		pthread_create(&tid,NULL,pthr,NULL);
		pthread_detach(tid);
	}
	close(sfd);
	return 0;
}
