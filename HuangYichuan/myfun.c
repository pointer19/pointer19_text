#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
void sys_err(const char* str)
{
perror(str);
exit(1);
}
int Socket(int domain ,int type,int protocol)
{
	int ret ;
	ret =socket(domain,type,protocol);
	if(ret ==-1)
	{
		sys_err("socket_error");
	}
	return ret;
}
