#include<stdio.h>                                                           
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s:%s",str,strerror(ernu));
	exit(1);
}
void fun()
{
	exit(0);//是结束整个进程
}
void* fun1()
{
	return NULL;//是返回给调用者，调用者是子线程
	//，返回后继续执行子线程没完成的东西
}
void* pthread_crea(void* arg)
{
	printf("-- pid=%d,tid=%lu\n",getpid(),pthread_self());    
//	fun();
	fun1();
	printf("*******");
	return NULL;//调用者是主线程，返回后代表子线程结束
	printf("!!!!!!!!!");
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_crea,NULL);
    
	
	sleep(2);
	printf(".................\n");  

	return 0;//调用者是bash，返回后bash会认为进程结束了
}
