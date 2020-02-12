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

void* pthread_crea(void* arg)
{
	int i=(int)arg;
	printf("--%d-- pid=%d,tid=%lu\n",i,getpid(),pthread_self());    
	return NULL;
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	int i;
	for(i=0;i<5;i++)
	{
	 	pthread_create(&tid,NULL,pthread_crea,(void*)i);
    }
	//由于是循环创建，虽然i的地址传过去了，但是当子线程访问的时候i可能变了
	//所以应该采用值传递
	sleep(1); //让主线程慢点死，否则释放完了内存空间，子线程主函数执行不了
	return 0;//跟exit(0)一样结束当前进程，释放内存空间
}
