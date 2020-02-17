
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

#define IP "127.0.0.1"
#define Port 8000

#ifndef _SOCKET_H
#define _SOCKET_H


int Socket(int domain, int type, int protocol);
int Accept(int sockfd, struct sockaddr_in *addr, socklen_t *addrlen);
void Listen(int sockfd, int backlog);
void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen);



#endif

