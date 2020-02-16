#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SER_PORT 8000
#define SER_ADDR "192.168.182.128"
#define BUF_SIZE 4096
void sys_err(char *str)
{
perror(str);
exit(1);

}

int main(int argc,char * argv[])
{
int lfd,cfd;
char buf[BUF_SIZE];

struct sockaddr_in ser_addr,cli_addr;//cli_ip;
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
cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
if(cfd==-1)
{
sys_err("accept error");
}
char dst[256];
printf("client is ok  ip:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
while(1)
{
	int rr=read(cfd,buf,sizeof(buf));

	write(STDOUT_FILENO,buf,rr);
	write(cfd,buf,rr);

}


return 0;

}
