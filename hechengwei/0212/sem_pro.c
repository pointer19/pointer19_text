#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#include<semaphore.h>

#define STR "123456\n"
void sys_err(char *str,int nu)
{
fprintf(stderr,"%s,%s\n",str,strerror(nu));
exit(10);

}

#define N 10
int arry[N]={0};
sem_t sem_space;
sem_t sem_full;

void*  pthread_producer(void *arg)
{
int i= 0;
while(1)
{
	sem_wait(&sem_space);
	arry[i]=rand() %500+1;
	printf("+++producer num =%d\n",arry[i]);
	sem_post(&sem_full);
	
	i=(i+1)%N;
	sleep(rand()%3);//qiehuan  cpu
}
return NULL;
}

void*  pthread_consumer(void *arg)
{
int i =0;
	while(2)
	{
		sem_wait(&sem_full);
		printf("-----------------------consumer num =%d\n",arry[i]);
		arry[i]=0;
		sem_post(&sem_space);

		i=(i+1)%N;
		sleep(rand()%2);

	}	



return NULL;
}

int main(int argc,char * argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	int ret ;
	sem_init(&sem_space,0,N);
	sem_init(&sem_full,0,0);

	ret =pthread_create(&tid2,NULL,pthread_producer,NULL);
	if(ret<0)
	{
		
		sys_err("pthread_creat error",ret);
	}
	ret =pthread_create(&tid2,NULL,pthread_consumer,NULL);
	{
	if(ret <0)
	{	
	sys_err("pthread _creat error",ret);
	}
	}
	ret=pthread_join(tid1,NULL);
	if(ret !=0)
	{
		sys_err("pthread error",ret);

	}
	ret =pthread_join(tid2,NULL);
	if(ret!=0)
	{
		sys_err("pthread error",ret);
	}

	ret=sem_destroy(&sem_space);
	if(ret!=0)	
	{
		sys_err("sem_destroy ",ret);
	}

	ret=sem_destroy(&sem_full);
	if(ret!=0)
	{	
	sys_err("sem_destroy",ret);
	}
	pthread_exit(NULL);






return 0;

}
