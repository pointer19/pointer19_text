#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#define PORT 8000
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void catch_child(int signum)
{
    pid_t wpid;
    while((wpid = waitpid(0, NULL, WNOHANG)) > 0)
    {
		printf("wait a child id = %d\n", wpid);
    }
}

int main(int argc, char* argv[])
{
    pid_t pid;
    int lfd = 0,cfd = 0;
    int ret;
    char buf[1024], client_IP[1024];
    struct sockaddr_in srv_addr, cli_addr;

    struct sigaction act;
    act.sa_handler = catch_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    ret = sigaction(SIGCHLD, &act, NULL);
    if(ret != 0)
    {
	sys_err("sigaction error: ");
    }
    
    bzero(&srv_addr, sizeof(srv_addr));
    bzero(&cli_addr, sizeof(cli_addr));
   
    socklen_t cli_addr_len;
    
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
	sys_err("socket error:");
    }
	//端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
    //绑定服务器地址结构
    ret = bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
	sys_err("bind error:");
    }
    //设置监听上限
    ret = listen(lfd, 128);
    if(ret == -1)
    {
	sys_err("listen error"); 	    
    }
    cli_addr_len = sizeof(cli_addr);
    while(1)
    {
	cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
	if(cfd == -1)
	{
	    sys_err("accept error"); 
	}
	
	printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
	pid = fork();
	if(pid < 0)
	{
	    sys_err("fork error");
	}
	else if(pid == 0)
	{
	    close(lfd);
	    printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
	    while(1)
	    {
		int ret1 = read(cfd, buf, sizeof(buf));
		if(ret1 == 0)
		{
		    exit(1);
		}
		write(cfd, buf, ret1);
		write(STDOUT_FILENO, buf, ret1);
	    }
	    close(cfd);

	}
	else
	{
	    close(cfd);
	   /* struct sigaction act;
	    act.sa_handler = catch_child;
	    sigemptyset(&act.sa_mask);
	    act.sa_flags = 0;

	    ret = sigaction(SIGCHLD, &act, NULL);
	    if(ret != 0)
	    {
		sys_err("sigaction error: ");
	    }*/
//	    continue;
	}
    }
    close(lfd);
    return 0;
}

