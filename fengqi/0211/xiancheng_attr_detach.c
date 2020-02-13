#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void* pthread_fun(void* arg)
{
		printf("hello 0211\n");
		return NULL;
}
void sys_err(char* str,int num)
{
		fprintf(stderr,"%s:%s\n",str,strerror(num));
		pthread_exit(NULL);
}
int main(int agrc,char* agrv[])
{
    pthread_t tid;
    pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&tid,&attr,pthread_fun,NULL);
    int ret=pthread_join(tid,NULL);
		if(ret!=0)
    {
       sys_err("pthread_join error",ret);
		}
		pthread_attr_destroy(&attr);
		pthread_exit(NULL);
}

