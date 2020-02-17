#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8000
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void *pthread_fun(void *arg)
{
    char buf[1024];
    int *cfd = (int *)arg;
    while(1)
    {
	int ret1 = read(*cfd, buf, sizeof(buf));
	if(ret1 == 0)
	{
	    exit(1);
	}
	write(*cfd, buf, ret1);
	write(STDOUT_FILENO, buf, ret1);
    }
    close(*cfd);
}

int main(int argc, char* argv[])
{
    pthread_t tid;
    int lfd = 0,cfd = 0;
    int ret;
    char client_IP[1024];
    struct sockaddr_in srv_addr, cli_addr;
 
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
    ret = bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
	sys_err("bind error:");
    }
    ret = listen(lfd, 128);
    if(ret == -1)
    {
	sys_err("listen error:"); 	    
    }
    cli_addr_len = sizeof(cli_addr);
    while(1)
    {
	//cli_addr_len = sizeof(cli_addr);
	cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
	if(cfd == -1)
	{
	    sys_err("accept error:"); 
	}
	
	printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
	ret = pthread_create(&tid, NULL, pthread_fun, (void *)&cfd);
	if(ret != 0)
	{
	    sys_err("pthread_create error:");
	}
	ret = pthread_detach(tid);
	if(ret != 0)
	{
	    sys_err("pthread_detach error:");
	}
    }
    close(lfd);
    close(cfd);
    return 0;
}

