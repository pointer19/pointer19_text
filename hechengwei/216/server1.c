#include"myfun1.h"
#include<signal.h>
#include<sys/wait.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.182.128"
#define BUF_SIZE 4096

void catch_child(int signu)
{
//printf("huishou");
pid_t wpid;
while((wpid=waitpid(-1,NULL,WNOHANG))>0)
{
	printf("wait pid =%d\n",wpid);
}

}
int main(int argc,char * argv[])
{
int lfd,cfd;
char buf[BUF_SIZE];

struct sockaddr_in ser_addr,cli_addr;//cli_ip;

struct sigaction act;
act.sa_handler = catch_child;
sigemptyset(&act.sa_mask);
act.sa_flags=SA_RESTART;;
sigaction(SIGCHLD,&act,NULL);

bzero(&ser_addr,sizeof(ser_addr));
bzero(&cli_addr,sizeof(cli_addr));
ser_addr.sin_family =AF_INET;
ser_addr.sin_port =htons(SER_PORT);
ser_addr.sin_addr.s_addr =htonl(INADDR_ANY);

lfd =socket(AF_INET,SOCK_STREAM,0);
if(lfd==-1)
{
sys_err("socket error ");
}
int opt=1;
setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));
int fh2=bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
if(fh2==-1)
{
(sys_err("bind error"));
}

int fh1=listen(lfd,128);
if(fh1==-1)
{
sys_err("listen error");
}

int cli_len=sizeof(cli_addr);
while(1)
{
	cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
	if(cfd==-1)
	{
	sys_err("accept error");
	}
	char dst[256];
	printf("client is ok  ip:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
	int pid =fork();
	if(pid<0)
	{
	sys_err("fork error");
	}
	else if(pid ==0)
	{
	close(lfd);
	while(1)
	{
	

	int rr=read(cfd,buf,sizeof(buf));
	if(rr==0)
	{
	exit(1);
	}

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
close(cfd);
return 0;

}
