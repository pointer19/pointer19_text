#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#define SER_PORT 8000
#define SER_ADDR "192.168.202.128"
#define BUF_SIZE 4096

void err(char * str)
{
	perror(str);
	exit(1);
}
int main(int argc,char* argv[])
{
	int maxfd;
	fd_set rset,aset;//一个新 一个旧
	FD_ZERO(&aset);//旧aset置0
	
	int lfd,cfd;
	char buf[4096];
	struct sockaddr_in ser_addr,cli_addr;

	bzero(&ser_addr,sizeof(ser_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(8000);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	lfd=socket(AF_INET,SOCK_STREAM,0);
	maxfd=lfd;//初始化max
	FD_SET(lfd,&aset);//把lfd赋给aset

	int opt=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
		
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(lfd,128);
	
	int sr;//select返回值
	int i=0;
	int rr;

	//定义数组装客户端的文件描述符
	int cli_arry[1024];
	int maxi=0;
	for(i=0;i<1024;i++)//置-1
	{
		cli_arry[i]=-1;
	}
	//把lfd放到数组里
	cli_arry[0]=lfd;
	maxi=1;
	
	while(1)

	{	
		rset=aset;
		sr=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr<0)
		{
			err("select error");
		}
		else if(sr>0)
		{
			if(FD_ISSET(lfd,&rset))//有客户端相连
			{
				int cli_len=sizeof(cli_addr);
				cfd=accept(lfd,(struct sockaddr*)&cli_addr,(socklen_t *)&cli_len);
				char dst[256];
				printf("client is ok IP:%s,port:%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,dst,sizeof(dst)),ntohs(cli_addr.sin_port));
				FD_SET(cfd,&aset);//把cfd添加到aset中
	
				for(i=1;i<1024;i++)
				{
					if(cli_arry[i]==-1)
					{
						cli_arry[i]=cfd;
						break;
					}
				}
				if(i==1024)
				{
					printf("客户端已经满了\n");	
				}
				if(i>maxi)
				{
					maxi=i;
				}

	
			if(cfd>maxfd)//变max
			{
				maxfd=cfd;
				}
			if(--sr==0)//sr==1
			{
				continue;	
			}
		}
		for(i=1;i<maxi+1;i++)//
		{
			if(cli_arry[i]<0)//
			{
				continue;	
			}
			if(FD_ISSET(cli_arry[i],&rset))
			{
				rr=read(cli_arry[i],buf,sizeof(buf));
				if(rr<0)
				{
					perror("read error");
					exit(1);
				}
				else if(rr==0)//对端关闭
				{
					printf("client lost\n");
					close(cli_arry[i]);
					FD_CLR(cli_arry[i],&aset);	//把i从监听队列里踢出去
					cli_arry[i]=-1;//从数组中移除
				}
				else
				{
					write(STDOUT_FILENO,buf,rr);		
					write(cli_arry[i],buf,rr);		
				}
				if(--sr==0)
				{
					break;	
				}
			}
		}
	}
}	
	return 0;	
}

