#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#define PORT 10086
#define MAXFD 1024
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
    bzero(buf, sizeof(buf));
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
    int epfd;//用于create
    struct epoll_event client, clients[MAXFD];//用于ctl,wait
    int event_count;//wait  满足事件个数
    int i;
    epfd = epoll_create(MAXFD);//epoll_create
    //初始化 client
    client.events = EPOLLIN;
    client.data.fd = lfd;
    //把lfd挂在文件系统中
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &client);
    while(1)
    {
	event_count = epoll_wait(epfd, clients, MAXFD, -1);
	if(event_count < 0)
	{
	    sys_err("epoll_wait error");
	}
	for(i = 0; i < event_count; i++)
	{
	    if(clients[i].events & EPOLLIN)
	    {
		if(clients[i].data.fd == lfd)
		{	    
		    cli_addr_len = sizeof(cli_addr);
		    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);	    
		    if(cfd == -1)
		    {
			sys_err("accept error:"); 
		    }
		    printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
		    client.events = EPOLLIN;
		    client.data.fd = cfd;

		    epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &client);
		}
		else
		{
		    int rr = read(clients[i].data.fd, buf, sizeof(buf));
		    if(rr < 0)
		    {
			sys_err("read error:");
		    }
		    else if(rr == 0)
		    {
			epoll_ctl(epfd, EPOLL_CTL_DEL, clients[i].data.fd, &client);
			close(clients[i].data.fd);
			printf("client close---------\n");
		    }
		    else
		    {
			write(STDOUT_FILENO, buf, rr);
		    }
		}
	    }
	}
    }
    return 0;
}
