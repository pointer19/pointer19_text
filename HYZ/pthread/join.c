#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void err(char * str,int ernu )
{
		fprintf(stderr,"%s,%s",str,strerror(ernu));
		exit(1);
}
struct student
{
	int nu;
	char name[256];	
	
};
void* pthread_fun(void* arg)
{
	printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
	return (void*)100;
	
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	struct student huhu;
	struct student * retval;

	pthread_create(&tid,NULL,pthread_fun,NULL);

	pthread_join(tid,(void**)&retval);//转返回的100

	printf("retval=%d\n",(int)retval);//阻塞回收的

	pthread_exit(NULL);
}

