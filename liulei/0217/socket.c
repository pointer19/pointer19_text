#include "socket.h"
#include "sys_error.h"
int Socket(int domain, int type, int protocol)
{
	int scfd = socket(domain, type, protocol);
	if(scfd == -1)
	{
		sys_error("socket error:");
	}
	else 
	{
		return scfd;
	}
}
int Accept(int sockfd, struct sockaddr_in *addr, socklen_t *addrlen)
{
	int acfd = accept(sockfd, (struct sockaddr*)addr, addrlen);
	if(acfd == -1)
	{
		sys_error("accept error:");
	}
	else
	{
		return acfd;
	}
}
void Listen(int sockfd, int backlog)
{
	int re = listen(sockfd, backlog);
	if(re == -1)
	{
		sys_error("listen error");
	}
}
void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen)
{
	int re = bind(sockfd, (struct sockaddr *)addr, addrlen);
	if(re == -1)
	{
		sys_error("bind error");
	}
}





	

	




