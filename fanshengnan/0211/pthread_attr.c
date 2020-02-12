#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include <pthread.h>
void sys_err(char *str ,int ernu )  //  是出错处理函数
{  
	printf("%s : %s", str ,strerror(ernu)) ;
	pthread_exit(NULL) ;
}
void *pthread_fun(void *arg) 
{
	printf ("child pthread\n") ;
	return NULL ;
}

int main( )        //主线程
{
	pthread_t tid;
	pthread_attr_t  attr;       //
	int ret1=pthread_attr_init(&attr);
	if(ret1 !=0)     //检查是否出错
	{
		sys_err("pthread_attr_init" ,ret1) ;
	}
	int ret2=pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_DETACHED)  ;    //创建进程分离的函数
	if(ret2 !=0)
	{
		sys_err("pthread_attr_init" ,ret2) ;
	}
	pthread_create(&tid ,&attr, pthread_fun, NULL) ;
//	pthread_detach(tid);
	
	int pjr=pthread_join(tid ,NULL)  ;    //回收进程
	if(pjr !=0)
	{
		sys_err("pthread_join error",pjr) ;
	}
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

