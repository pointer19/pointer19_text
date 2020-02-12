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
    struct student* qinyuan;
	qinyuan=malloc(sizeof(qinyuan));
	qinyuan->nu=123;
	strcpy(qinyuan->name,"qinyuan");
	return qinyuan;
}


int main(int argc,char* argv[])
{
	
	pthread_t tid;
	struct student *retval;
	pthread_create(&tid,NULL,pthread_crea,NULL);
	pthread_join(tid,(void**)&retval);
	
	
	printf("retval  n=%d  name=%s\n",retval->nu,retval->name);
	
	pthread_exit(NULL);

}
