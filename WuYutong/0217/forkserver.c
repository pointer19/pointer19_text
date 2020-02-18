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
#define d 8000

void Waitpid(int sig)
{
	while(waitpid(0, NULL, WNOHANG) > 0)
	{
		printf("fork wait\n");		
	}
}
int main(int argc,char* argv[])
{
	struct sigaction act;
	act.sa_handler = Waitpid;
	act.sa_flags = SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
	int sfd,cfd;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}
	struct sockaddr_in addr, caddr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(d);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&caddr, sizeof(struct sockaddr_in));
	bind(sfd, (struct sockaddr*)&addr, (socklen_t)sizeof(addr));
	listen(sfd, 128);
	char buf[4096] = { 0 };
	while(1)
	{	
		int size = sizeof(caddr);
		cfd = accept(sfd, (struct sockaddr*)&caddr, (socklen_t*)&size);	
		if(cfd < 0)
		{
			perror("socket");
			continue;
		}
		pid_t pid = fork();
		if(pid < 0)
		{
			perror("fork error\n");
			continue;	
		}
		else if(pid == 0)
		{
			close(sfd);
			int irr;
			while((irr = read(cfd, buf, 1024)) > 0)
			{
				write(STDOUT_FILENO, buf, irr);
				write(cfd,buf,irr);
			}
			close(cfd);
			printf("child fork socket close\n");
			return 0;
		}
		else
		{
			close(cfd);
			continue;	
		}
	}
	close(sfd);
	return 0;
}

