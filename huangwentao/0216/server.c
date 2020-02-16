#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define d 8000
int main(int argc,char* argv[])
{
	int sfd,cfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("socket");
		exit(0);
	}
	struct sockaddr_in addr,caddr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(d);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&caddr,sizeof(struct sockaddr_in));
	bind(sfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
	listen(sfd,128);
	int size=sizeof(caddr);
	cfd=accept(sfd,(struct sockaddr*)&caddr,(socklen_t*)&size);
	if(cfd<0)
	{
		perror("socket");
		exit(0);	
	}
	char buf[4096]={0};
	while(1)
	{
	 int rr=read(cfd,buf,1024);
	 	write(STDOUT_FILENO,buf,rr);
		write(cfd,buf,rr);
	}
	close(sfd);
return 0;
}

