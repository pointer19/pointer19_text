#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void err(char * str,int ernu )
{
		fprintf(stderr,"%s,%s\n",str,strerror(ernu));
	   pthread_exit(NULL);
}
void* pthread_fun(void* arg)
{
	printf("child pthread\n");
	return NULL;
	
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	pthread_attr_t attr;//创建结构体
	int ret1=pthread_attr_init(&attr);//初始化
	if(ret1!=0)
	{
		err("pthread_attr_init error",ret1);	
	}
	int ret2=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//!=0 设置分离属性
	if(ret2!=0)
	{
		err("pthread_attr_setdetachstate",ret2);
	}
	pthread_create(&tid,&attr,pthread_fun,NULL);//创建线程
	//pthread_detach(tid);
	
	int pjr=pthread_join(tid,NULL);
    if(pjr!=0)	
	{
		err("pthread_join",pjr);	
	}
	pthread_attr_destroy(&attr);//销毁线程属性结构体变量
	pthread_exit(NULL);
}

