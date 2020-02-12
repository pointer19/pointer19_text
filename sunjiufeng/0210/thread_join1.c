#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

struct student
{
	int nu;
	char name[256];
};

void sys_err(char *str, int ernu)
{
	fprintf(stderr, "%s: %s\n", str, strerror(ernu));
	exit(1);
}

void *pthread_fun(void *arg)
{
	struct student *sjf = (struct student*)malloc(sizeof(struct student));
	sjf->nu = 100;
	strcpy(sjf->name, "sjf");
	return (void*)sjf;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	struct student *retval;
	int ptr = pthread_create(&tid, NULL, pthread_fun, NULL);
	if(ptr != 0)
	{
		sys_err("create error", ptr);
	}

	pthread_join(tid, (void**)&retval);
	printf("nu = %d, name = %s\n", retval->nu, retval->name);

	sleep(1);
	pthread_exit(NULL);
}

