#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 10086
#define SER_ADDR "127.0.0.1"
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char* argv[])
{
    int cfd;
    int ret = 0;//jian cha fan hui zhi
    char buf[1024];
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SER_ADDR, &addr.sin_addr.s_addr);
 
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1)
    {
	sys_err("socket error:");
    }
    
    ret = connect(cfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1)
    {
	sys_err("connect error:"); 	    
    }
    
    while(1)
    {
	
	int ret1 = read(STDIN_FILENO, buf, sizeof(buf));
	//write(cfd, buf, ret1);
	//int ret2 = read(cfd, buf, sizeof(buf)); 
	write(STDOUT_FILENO, buf, ret1);
	write(cfd, buf, ret1);
    }

   // close(cfd);
    return 0;
}

