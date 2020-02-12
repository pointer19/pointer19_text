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
int a=20;
void *pthread_fun(void *arg)     //线程创建成功后再此函数中执行线程的相关操作
{
	a=30;
	printf ("pid =%d ,tid=%lu\n",getpid( ), pthread_self( )) ;   //lu是无符号的长整形
}

int main( )
{
	pthread_t tid;
	pthread_create(&tid ,NULL , pthread_fun, NULL) ;
	sleep(1);	
	printf("a=%d\n",a) ;
	return 0;
}

