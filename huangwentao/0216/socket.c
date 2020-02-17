#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#define d 8000
int main(int argc,char* argv[])
{
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(d);
	int adr;
	inet_pton(AF_INET,"127.0.0.1",(void*)&adr);
	addr.sin_addr.s_addr=adr;
	int cr=connect(sfd,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
	if(cr<0) 
	{perror("connet error");exit(0);}
	char buf[4096]={0};
	while(1)
	{
		int rr=read(STDIN_FILENO,buf,1024);
		write(sfd,buf,rr);
			
		rr=read(sfd,buf,1024);
		write(STDOUT_FILENO,buf,rr);
	}
	close(sfd);

return 0;
}

