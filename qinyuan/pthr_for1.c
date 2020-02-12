#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
void sys_error(char *str,int ernu)
{
	fprintf(stderr,"%s:,%s:",str,strerror(ernu));
	exit(1);
}
void* pthread_fun(void *arg)
{
	int i=(int)arg;//这样转换不会造成数据流失
	printf("--%dth--pid=%d,tid=%lu\n",i,getpid(),pthread_self());
	return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	int i;
	for(i=0;i<5;i++)
	{
		pthread_create(&tid,NULL,pthread_fun,(void *)i);
	}

	sleep(1);
	return 0;
}

