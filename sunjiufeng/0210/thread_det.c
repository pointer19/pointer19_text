#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

void sys_err(char* str, int ernu)
{
	fprintf(stderr, "%s: %s\n", str, strerror(ernu));	
}

void *thread_fun(void *arg)
{
	printf("+++++++\n");
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int *retval;

	int ptr = pthread_create(&tid, NULL, thread_fun, NULL);
	if(ptr != 0)
	{
		sys_err("create error", ptr);
	}

	int jor = pthread_detach(tid);
	if(jor != 0)
	{
		sys_err("detach error", jor);
	}

	pthread_join(tid, (void**)&retval);
	printf("-------------\n");
	pthread_exit(NULL);


	return 0;
}

