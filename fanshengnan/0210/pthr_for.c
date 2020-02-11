#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include <pthread.h>
void sys_err(char *str ,int ernu )
{
	strerror(ernu);      //此函数用来翻译错误号
	fprintf("%s : %s", str ,strerror(ernu)) ;
	exit(1) ;
}
void *pthread_fun(void *arg)     //线程创建成功后再此函数中执行线程的相关操作
{
	sleep(1);
	int i=(int *)arg;
	printf ("%dth---,pid =%d ,tid=%lu\n",i,getpid(), pthread_self()) ;   //lu是无符号的长整形
}

int main( )
{
	int i;
	pthread_t tid;
	for(i=0;i<5;i++)
	{
		pthread_create(&tid ,NULL , pthread_fun, (void*)i) ;
	
	}
	sleep(2);   
	return 0;
}

