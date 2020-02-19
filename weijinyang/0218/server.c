#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#define PORT 10086
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char* argv[])
{
    int lfd = 0,cfd = 0;
    int ret;//jian cha fan hui zhi
    char buf[1024], client_IP[1024];
    struct sockaddr_in addr, cli_addr;

    bzero(&addr, sizeof(addr));
    bzero(&cli_addr, sizeof(cli_addr));
   
    socklen_t cli_addr_len;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(lfd == -1)
    {
	sys_err("socket error:");
    }
    
    ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1)
    {
	sys_err("bind error:");
    }

    ret = listen(lfd, 128);
    if(ret == -1)
    {
	sys_err("listen error:"); 	    
    }
    int i;
    int maxi = 0;//记录数组用到了第多少个
    struct pollfd pfd[1024];// poll jie gou ti shu zu
    bzero(pfd, sizeof(pfd));//shu zu zhi ling
    pfd[0].fd = lfd;
    pfd[0].events = POLLIN;
   // pfd[0].revents = 0;
    for(i = 1; i < 1024; i++)
    {
	pfd[i].fd = -1;//除了lfd都设置为-1
    }
    int pr;
    while(1)
    {
	pr = poll(pfd, maxi+1, -1);//传maxi+1，maxi是数组下标
	if(pr < 0)
	{
	    sys_err("poll error:");
	}
	else if(pfd[0].revents & POLLIN)//判断lfd是否有pollin，有代表有客户端连接，accept
	{
	    cli_addr_len = sizeof(cli_addr);
	    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);	    
	    if(cfd == -1)
	    {
		sys_err("accept error:"); 
	    }
	    printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
	    for(i = 1; i < 1024; i++)//遍历数组
	    {
		if(pfd[i].fd < 0)//如果值为-1,
		{//把新的cfd添加进去，
		    pfd[i].fd = cfd;
		    pfd[i].events = POLLIN;
		    break;
		}
	    }
	    if(i == 1024)
	    {
		printf("client is full.\n");
		exit(1);
	    }
	    if(i > maxi)//更新maxi
	    {
		maxi = i;
	    }
	    if(--pr == 0)//如果没有待处理事件
	    {
		continue;
	    }
	}
	for(i = 1; i <= maxi; i++)//如果不是lfd,是cfd
	{
	    if(pfd[i].fd == -1)//遍历找，-1就跳过
	    {
		continue;
	    }
	    else if(pfd[i].revents & POLLIN)//如果里面事件满足
	    {
		ret = read(pfd[i].fd, buf, sizeof(buf));//读
		if(ret < 0)//读出错
		{
		    sys_err("read error:");
		}
		else if(ret == 0)//对端关闭
		{
		    printf("客户端关闭:\n");
		    close(pfd[i].fd);//关闭文件描述符
		    pfd[i].fd = -1;//数组置-1
		}
		else//xie
		{
		    write(STDOUT_FILENO, buf, ret);
		   // write(pfd[i].fd, buf, ret);
		}
		if(--pr == 0)
		{
		    break;
		}
	    }
	}
    }
    return 0;
}

