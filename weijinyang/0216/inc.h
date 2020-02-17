
#ifndef _MYFUN_H_
#define _MYFUN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
void sys_err(const char *str);
int Socket(int domain, int type, int protocol);




#endif
