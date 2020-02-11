#include<stdio.h>                                                           
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
struct student
{
	int nu;
	char name[20];
};

void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s:%s",str,strerror(ernu));
	exit(1);
}

void* pthread_crea(void* arg)
{
	struct student* sundi=(struct student*)arg;
	sundi->nu=123;
	strcpy(sundi->name,"sundi");
	return (void*)sundi;
}


int main(int argc,char* argv[])
{
	
	pthread_t tid;
	struct student *retval;
	struct student sundi;
	
	pthread_create(&tid,NULL,pthread_crea,(void*)&sundi);
	pthread_join(tid,(void**)&retval);
	
	
	printf("retval  n=%d  name=%s\n",retval->nu,retval->name);
	
	pthread_exit(NULL);
}
