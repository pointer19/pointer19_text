
#ifndef _MYFUN_H_
#define _MYFUN_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int Socket(int domain,int type,int protocol);

#endif
