#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void sys_err(char* str,int erno)
{
	fprintf(stderr,"%s:%s\n",str,strerror(erno));
	exit(1);
}

void* pthread_fun(void* arg)
{
	printf("tid = %lu\n",pthread_self());
	return (void*)100;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	pthread_create(&tid,&attr,pthread_fun,NULL);
	sleep(1);
	int pj=pthread_join(tid,NULL);
	if(pj!=0)
	{
		sys_err("pthread_join",pj);
	}
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

