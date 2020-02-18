#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#ifndef _SIGNAL_H_
#define _SIGNAL_H_


void c_signal(int a);
void Signal();

#endif
