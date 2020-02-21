#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
int main()
{
	int fd[2];
	pipe(fd);         //管道
	pid_t pid=fork();   //创建进程
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid==0)      //子进程
	{
		close(fd[0]);     //关闭读端
		int i;
		for(i=0;i<10;i++)
		{
			dprintf(fd[1],"----%d----\n",i); //往管道的写端写东西
			sleep(3);   //表示每隔3秒往管道写东西
		}
		close(fd[1]);     //关闭写端
	}
	else
	{
		close(fd[1]);       //关闭写端
		int epfd=epoll_create(16);   //创建文件系统
		struct epoll_event exp, exps[10];
		exp.events= EPOLLIN | EPOLLET;  //监测文件描述符
		exp.data.fd=fd[0];

		fcntl(fd[0],F_SETFL,O_NONBLOCK); //将读管道设置为非阻塞方式

		epoll_ctl(epfd,EPOLL_CTL_ADD,fd[0],&exp);  //挂载
		int ret , rr;
		char buf[32];
		while(1)
		{
			ret=epoll_wait(epfd ,exps,10,-1);  //读取
			if(ret<0)
			{
				perror("epoll_ctl error");
				exit(1);
			}
			else
			{
				while(1)
				{
					rr=read(exps[0].data.fd ,buf ,5); 
					if(rr<0)
					{
						if(errno==EAGAIN)
						{				
							break;
						}

						perror("read error");
						exit(1);
					}
					else if(rr==0)
					{
						printf("对端关闭\n");
						exit(1);
					}
					else
					{
						write(STDOUT_FILENO,buf,rr);
					}
			   	}
			}
		   }
		
	}
	return 0;
}


