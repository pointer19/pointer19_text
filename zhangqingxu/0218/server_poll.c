#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<poll.h>

#define SER_PORT 8886
int main(int argc, char* argv[])
{
	int sfd,cfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in ser_addr,cli_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(SER_PORT);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int cli_len = sizeof(cli_addr);
	bzero(&cli_addr,cli_len);

	int opt=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(void* )&opt,sizeof(opt));

	bind(sfd,(struct sockaddr* )&ser_addr,sizeof(ser_addr));
	listen(sfd,128);


	struct pollfd fds[1024];   //定义结构体数组
	bzero(fds,1024);           //初始化
	fds[0].fd=sfd;
	fds[0].events=POLLIN;
	for(int i=1;i<1024;i++)
	{
		fds[i].fd=-1;      //将未用的位置，文件描述符设置成-1
	}
	int buf[1024];
	int rr;
	int pr;
	int maxindex=0;     //最大下标

	while(1)
	{
		pr = poll(fds,maxindex+1,-1);
		if(pr<0)
		{
			perror("poll error");
			exit(1);
		}
		else if(fds[0].revents & POLLIN)
		{
			cfd=accept(sfd,(struct sockaddr*)&cli_addr,&cli_len);
			char buf1[100];
			printf("client ip=%s,port= %d\n",inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,buf1,100),ntohs(cli_addr.sin_port));

			for(int i=1;i<1024;i++)
			{
				if(fds[i].fd==-1)
				{
					fds[i].fd=cfd;
					fds[i].events=POLLIN;   //不写打印不出来
					if(maxindex<i)
					{
						maxindex=i;   //更新最大下标
					}
					break;
				}
			}
			if(--pr==0)
			{
				continue;
			}
		}
		for(int i = 1;i <= maxindex;i++)    //处理通信
		{
			if(fds[i].fd==-1)
			{
				continue;
			}
			if(fds[i].revents & POLLIN)
			{
				printf("-----------\n");
				rr=read(fds[i].fd,buf,sizeof(buf));
				if(rr<0)
				{
					perror("read error");
					exit(1);
				}
				if(rr==0)
				{
					printf("client close\n");
					close(fds[i].fd);
					fds[i].fd=-1;
				}
				else
				{
					write(STDOUT_FILENO,buf,rr);
				}
				if(--pr==0)
				{
					break;
				}
			}
		}
	}
	return 0;
}

