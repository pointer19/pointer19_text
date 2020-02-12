#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void* pthread_d(void* num)
{
	printf("hahaha\n");
	return NULL;
}
int main(int argc,char* argv[])
{
pthread_t tid;
pthread_create(&tid,NULL,pthread_d,NULL);
pthread_detach(tid);
pthread_exit(NULL);
}

