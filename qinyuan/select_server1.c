#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/select.h>
#define MAXLINE 80
#define SER_PORT 8000
#define MAXFD 1024
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
int main(int argc, char *argv[])
{
	int ret;
	int lfd,cfd;
	fd_set rset, allset;
	int maxfd;
	char buf[MAXLINE];
	struct sockaddr_in ser_addr, cli_addr;
	bzero(&ser_addr,sizeof(ser_addr));
	bzero(&cli_addr,sizeof(cli_addr));
	FD_ZERO(&rset);
	FD_ZERO(&allset);
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ser_addr.sin_port = htons(8000);
	lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1)
	{
		sys_err("create socket error");
	}
	maxfd = lfd;
	FD_SET(lfd, &allset);
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	ret = bind(lfd, (struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if(ret == -1)
	{
		sys_err("bind error");
	}
	ret = listen(lfd, 128);
	if(ret == -1)
	{
		sys_err("listen error");
	}
	int size_cli = sizeof(cli_addr);
	int nu,rn,i;

	int cli_arry[MAXFD];
	int maxi = 0;
	for(i=0;i<MAXFD;i++)
	{
		cli_arry[i]=-1;
	}
	cli_arry[0] = lfd;




	while(1)
	{
		rset = allset;
		nu = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(nu < 0)
		{
			sys_err("select error");
		}
		else if(nu > 0)
		{
			if(FD_ISSET(lfd, &rset))
			{
				cfd = accept(lfd, (struct sockaddr*)&cli_addr, (socklen_t *)&size_cli);
				if(cfd == -1)
				{
					sys_err("accept error");
				}
				char info[256];
				printf("client is ok IP:%s ,  port:%d\n",inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr,info,sizeof(info)), ntohs(cli_addr.sin_port));
				FD_SET(cfd, &allset);
				for(i = 1;i < MAXFD;i++)
				{
					if(cli_arry[i] == -1)
					{
						cli_arry[i]=cfd;
						break;
					}
				}
				if(i == MAXFD)
				{
					printf("too many client\n");
					exit(1);
				}

				if(i > maxi)
				{
					maxi = i;
				}
				if(cfd > maxfd)
				{
					maxfd = cfd;
				}
				if(--nu == 0)
				{
					continue;
				}
			}
			for(i = 1; i < maxi + 1; i++)
			{
				if(cli_arry[i]<0)
				{
					continue;
				}
			 	else if(FD_ISSET(cli_arry[i], &rset))
				{
					rn = read(cli_arry[i], buf, sizeof(buf));
					if(rn < 0)
					{
						sys_err("read error");
					}
					else if(rn == 0)
					{
						close(cli_arry[i]);
						printf("client is miss\n");
						FD_CLR(cli_arry[i] , &allset);
						cli_arry[i]=-1;
					}
					else
					{
						write(STDOUT_FILENO, buf, rn);
						write(cli_arry[i], buf, rn);
					}
					if(--nu == 0)
					{
						break;
					}
				}
			}
		}
	}
	return 0;
}
