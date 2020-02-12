#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H 500

int a=0;
pthread_rwlock_t rwlock;


void* pthread_wfun(void* arg)
{
		while(1)
		{
        pthread_rwlock_wrlock(&rwlock);
				int b=a;
				a++;
				printf("----write %d----old a=%d,new a=%d\n",(int)arg,b,a);
				pthread_rwlock_unlock(&rwlock);
				sleep(2);
		}
		return NULL;
}
void* pthread_rfun(void* arg)
{
		while(1)
		{
        pthread_rwlock_rdlock(&rwlock);
				printf("----read %d----a=%d\n",(int)arg,a);
				pthread_rwlock_unlock(&rwlock);
				sleep(1);
		}
		return NULL;
}
void sys_err(char* str,int num)
{
		fprintf(stderr,"%s:%s\n",str,strerror(num));
		pthread_exit(NULL);
}
int main(int agrc,char* agrv[])
{
    pthread_t tid[10];
		pthread_rwlock_init(&rwlock,NULL);
		int i;
		for(i=0;i<4;i++)
		{
      pthread_create(&tid[i],NULL,pthread_wfun,(void*)i);
		  pthread_detach(tid);
		}
		for(i=4;i<10;i++)
		{
			pthread_create(&tid[i],NULL,pthread_rfun,(void*)i);
			pthread_detach(tid);
		}
    pthread_rwlock_destroy(&rwlock);
		pthread_exit(NULL);
}

