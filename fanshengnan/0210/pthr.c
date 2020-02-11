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
	printf ("pid =%d ,tid=%lu\n",getpid( ), pthread_self( )) ;   //lu是无符号的长整形
}

int main( )
{
	pid_t pid=getpid( );   //获取进程 ID的值
	pthread_t tid1=pthread_self( );   //获取线程ID的值
	pthread_t tid;
	pthread_create(&tid ,NULL , pthread_fun, NULL) ;
	
	printf("pid =%d ,tid1=%lu\n",pid ,tid1) ;
	sleep(1);   
	return 0;
}

