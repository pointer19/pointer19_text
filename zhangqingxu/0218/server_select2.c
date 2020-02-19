#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>


#define SER_PORT 8886
int main(int argc, char* argv[])
{
	int sfd,cfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(SER_PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	int cli_len=sizeof(cli_addr);
	bzero(&cli_addr,cli_len);

	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void* )&opt, sizeof(opt));

	bind(sfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	listen(sfd,128);


	int arry[1024];   //定义存放文件描述符的数组
	for(int i=0;i<1024;i++)
	{
		arry[i]=-1;   //全置为-1
	}
	arry[0]=sfd;
	int maxindex=1;
	int buf[1024];
	int sr,rr;
	fd_set rset,aset;
	FD_ZERO(&aset);
	FD_SET(sfd,&aset);
	int maxfd=sfd;
	while(1)
	{
		rset=aset;
		sr=select(maxfd+1,&rset,NULL,NULL,NULL);
		if(sr<0)
		{
			perror("select error");
			exit(1);
		}
		else if(sr>0)
		{
			if(FD_ISSET(arry[0],&rset))
			{
				cfd=accept(sfd,(struct sockaddr*)&cli_addr,&cli_len);
				if(cfd<0)
				{
					perror("accept error");
					exit(1);
				}
				char buf1[100];
				printf("client ip=%s,port=%d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,buf1,100),ntohs(cli_addr.sin_port));
				FD_SET(cfd,&aset);
				
				for(int j=0;j<1024;j++)   //将CFD存放到数组中
				{
					if(arry[j]==-1)
					{
						arry[j]=cfd;
						if(j>maxindex)
						{
							maxindex=j;
						}
						if(arry[j]>maxfd)
						{
							maxfd=arry[j];
						}
						break;
					}
				}
				if(--sr==0)
				{
					continue;
				}
			}
			for(int i=1;i<=maxindex;i++)  //便利数组
			{
				if(FD_ISSET(arry[i],&rset))
				{
					rr=read(arry[i],buf,sizeof(buf));
					if(rr==-1)
					{
						perror("read error");
						exit(1);
					}
					else if(rr==0)
					{
						FD_CLR(arry[i],&aset);
						close(arry[i]);
						arry[i]=-1;
						continue;
					}
					write(STDOUT_FILENO,buf,rr);
					write(arry[i],buf,rr);
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

