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
void* fun()
{
	return NULL;	
}
void* pthread_fun(void* arg)
{
	printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
	fun();
	printf("---\n");//可以打印 fun结束的是自身
	return NULL;//结束自身，返回给调用者

	printf("+++\n");//不可以
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);

	pthread_exit(NULL);//用来结束线程	

	return 0;//主线程结束
}

