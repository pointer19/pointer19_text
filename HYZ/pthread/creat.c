#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void err(char * str,int ernu)
{
		fprintf(stderr,"%s,%s",str,strerror(ernu));
		exit(1);
}
void* pthread_fun(void* arg)//线程的子函数
{
	sleep(1);
	int i=(int)arg;
	printf("---%dtn---,pid=%d,tid1=%lu\n",i,getpid(),pthread_self());//%lu
	return NULL;
}
int main(int argc,char* argv[])//线程的主线程
{
	pid_t pid=getpid();
	pthread_t tid=pthread_self();

	pthread_t tid1;
	int i;
	for(i=0;i<5;i++)
	{
		pthread_create(&tid1,NULL,pthread_fun,(void*)i);
		//(void*)&i 传参不能用&，当sleep1之后，都变成5th
		//要用值传递，4字节变8字节变4字节后不会发生数据丢失
	}
	printf("pid=%d,tid2=%lu\n",pid,tid);	
	sleep(2);
	
	return 0;
}

