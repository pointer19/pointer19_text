#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 10086
void sys_err(char *str)
{
    perror(str);
    exit(1);
}


int main(int argc, char* argv[])
{

    int lfd = 0,cfd = 0, i = 0;
    int ret;//jian cha fan hui zhi
    char buf[1024], client_IP[1024];
    struct sockaddr_in srv_addr, cli_addr;
    int maxfd; 
    fd_set rset, aset;
    FD_ZERO(&aset); 
    
    bzero(&srv_addr, sizeof(srv_addr));
    bzero(&cli_addr, sizeof(cli_addr));
   
    socklen_t cli_addr_len;
    
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(PORT);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    maxfd = lfd;

    FD_SET(lfd, &aset);
    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
    //绑定服务器地址结构
    bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    //设置监听上限
    listen(lfd, 128);
      
    int sr;
    while(1)
    {
	rset = aset;
	sr = select(maxfd + 1, &rset, NULL, NULL, NULL);
	if(sr < 0)
	{
	    sys_err("select error:");
	}
	else if(sr > 0)
	{
	    if(FD_ISSET(lfd, &rset))
	    {
                cli_addr_len = sizeof(cli_addr);
		cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		printf("client IP = %s, port = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
		FD_SET(cfd, &aset);
		if(maxfd < cfd)
		{
		    maxfd = cfd;
		}
		if((--sr) == 0)
		{
		    continue;
		}
	    }
	    for(i = lfd+1; i <= maxfd; i++)
	    {
	        if(FD_ISSET(i, &rset))
		{
		    int rr = read(i, buf, 1024);
		    if(rr == 0)
		    {
		        printf("----client close\n");
		        close(i);
		        FD_CLR(i, &aset);
		    }
		    else if(rr < 0)
		    {
		        sys_err("read error:");
		    }
		    else
		    {
		        write(i, buf, rr);
		        write(STDOUT_FILENO, buf, rr);
		    }
		    if((--sr) == 0)
		    {
		        break;
		    }
		}
	    }   
	}   
    }
    return 0;
}

