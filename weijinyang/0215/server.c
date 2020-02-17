#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    
    cli_addr_len = sizeof(cli_addr);
    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if(cfd == -1)
    {
        sys_err("accept error:"); 
    }
   
    printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
    while(1)
    {	
	int ret1 = read(cfd, buf, sizeof(buf));	
	write(STDOUT_FILENO, buf, ret1);
//	write(cfd, buf, ret1);
    }
    close(lfd);
    close(cfd);
    return 0;
}

