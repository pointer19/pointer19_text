#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void* pthread_crea(void* arg)
{
	int i=((int)arg);
	printf("--%d-- pid=%d,tid=%lu\n",i,getpid(),pthread_self());	
}
int main(int argc,char* argv[])
{
pthread_t tid;
for(int i=0;i<=5;i++)
{
	pthread_create(&tid,NULL,pthread_crea,(void*)i);
}
sleep(2);//防止主線程先走完
return 0;
}

