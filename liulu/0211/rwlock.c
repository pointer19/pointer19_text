#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define H 10000
int a = 0;
pthread_rwlock_t rwlock;

void sys_err(char* str, int nu)
{
    fprintf(stderr, "%s:%s\n", str, strerror(nu));
    exit(1);
}

void* pthread_rfun(char* arg)
{
    while(1)
    {
	pthread_rwlock_rdlock(&rwlock);
	printf("read %dth, a = %d\n", (int)arg, a);
	pthread_rwlock_unlock(&rwlock);
	sleep(1);
    }
    return NULL;
}
void* pthread_wfun(char* arg)
{
        while(1)
	{
	    pthread_rwlock_wrlock(&rwlock);
	    int b = a;
	    a++;
	    printf("write %dth,old a = %d,new a = %d\n", (int)arg, b, a); 
	    pthread_rwlock_unlock(&rwlock);	
	    sleep(2);	    
	}   
	return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid[10];
    pthread_rwlock_init(&rwlock, NULL);

    int i;
    for(i = 0; i < 4; i++)
    {
	pthread_create(&tid[i], NULL, pthread_wfun, (void*)i);
	pthread_detach(tid);
    }
    for(i = 4; i < 10; i++)
    {
	pthread_create(&tid[i], NULL, pthread_rfun, (void*)i);
	pthread_detach(tid);
    }
    pthread_rwlock_destroy(&rwlock);
    pthread_exit(NULL);
}

