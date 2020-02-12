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
	struct student *sjf = (struct student*)arg;
	sjf->nu = 100;
	strcpy(sjf->name, "sjf");
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	struct student *retval;
	struct student sjf;
	int ptr = pthread_create(&tid, NULL, pthread_fun, (void*)&sjf);
	if(ptr != 0)
	{
		sys_err("create error", ptr);
	}

	pthread_join(tid, (void**)&retval);
	printf("nu = %d, name = %s\n", sjf.nu, sjf.name);

	sleep(1);
	pthread_exit(NULL);
}

