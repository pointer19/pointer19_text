#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>
#include<string.h>
void *pthread(void *a)
{
	int b=(int)a;
	printf("%d,%lu\n",b,pthread_self());
		
	return NULL;
}
void sys_err(char *str, int singal)
{
	fprintf(stderr, "%s,%s",str,strerror(singal));
	exit(1);
}

int main(int argc, char *argv[])
{
	pthread_t tid;
//	tid = pthread_self();
	for(int i=0;i<5;i++)
	{
		pthread_create(&tid, NULL, pthread, (void*)i); 
	}
	sleep(1);
/*	int a;
	pthread_join(tid, (void **)a);
	printf("%d\n",(int)a);*/
	return 0;
}

