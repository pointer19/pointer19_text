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
	printf("-- pid=%d,tid=%lu\n",getpid(),pthread_self());    
	return NULL;
}
int main(int argc,char* argv[])
{
	pid_t pid=getpid();
	pthread_t tid1=pthread_self();
	
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_crea,NULL);
    printf("pid=%d, tid1=%lu\n",pid,tid1);  

	sleep(1); //让主线程慢点死，否则释放完了内存空间，子线程主函数执行不了
	return 0;//跟exit(0)一样结束当前进程，释放内存空间
}
