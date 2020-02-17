#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define SER_PORT 8001         //端口号
#define BUF_SIZE 4096         //缓冲区 
#include<signal.h>
void sys_err(char* str)
{
	perror(str);
	exit(1);	
}
void catch_child(int signu)
{
	pid_t wpid;
	while((wpid=waitpid(-1,NULL,WNOHANG))>0)
	{   
		printf("waitpid=%d\n",wpid);
	}   
}



int main(int argc,char* argv[])
{
	int lfd,cfd;
	char*buf[BUF_SIZE];
	struct sockaddr_in ser_addr,cli_addr;

	struct sigaction act;
	act.sa_handler=catch_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGCHLD,&act,NULL);


	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bzero(&cli_addr,sizeof(cli_addr));

	lfd=socket(AF_INET,SOCK_STREAM,0);

	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	if(lfd==-1)
	{
		sys_err("socket error:" );		
	}
	int bin=bind(lfd, (struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(bin==-1)
	{
		sys_err("bind  error:" );  	
	}
	
	int lis=listen(lfd,128);
    if(lis==-1)
	{
		sys_err("listen  error:" );  
	}
	while(1)
	{
	
		int cli_len=sizeof(cli_addr);
		cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);
		if(cfd==-1)
		{
			sys_err("accept error:" );  	
		}	
	
		//	printf("client ip=%d port=%d\n",ntohl(cli_addr.sin_addr.s_addr),ntohs(cli_addr.sin_port));	

		char ip[50];
		printf("client ip=%s port=%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,ip,sizeof(ip)),ntohs(cli_addr.sin_port));

	
		int pid=fork();
		if(pid<0)
		{
			sys_err("fork error:");
		}
		else if(pid==0)
		{
			close(lfd);	
			while(1)
			{
				int rr=read(cfd,buf,sizeof(buf));
			//	if(rr==0)
			//	{
			//		exit(1);
			//	}	
				write(STDOUT_FILENO,buf,rr);
				write(cfd,buf,rr);

			}
			close(cfd);

		}
		else
		{
			close(cfd);	
		}

	}


	close(lfd);
	 return 0;
}     

