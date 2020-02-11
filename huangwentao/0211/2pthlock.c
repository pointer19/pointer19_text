#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
int a=0;
pthread_mutex_t mutex;
void* add(void* argc)
{
 for(int i=0;i<10000;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("---%d----\n",a);
		pthread_mutex_unlock(&mutex);
	}
	
}
int main(int argc,char* argv[])
{
	pthread_mutex_init(&mutex,NULL);
pthread_t tid;
pthread_create(&tid,NULL,add,NULL);
pthread_detach(tid);


 for(int i=0;i<10000;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("---%d----\n",a);
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}

