#ifndef _MYFYN_H
#define _MYFUN_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>

void sys_err(const char *str);
int Socket(int domain,int type,int protocol);

#endif

