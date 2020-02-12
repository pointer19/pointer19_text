#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s: %s\n",str,strerror(ernu));
	exit(1);
}

void* pthread_fun(void* arg)  //子线程的主函数
{
	sleep(2);
	printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
	return (void*) 100;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	void* retval;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	//(void**)&retval作用是接住子线程退出主函数的返回值
	int pjno=pthread_join(tid,&retval);
	if(pjno!=0)
	{
		sys_err("pthread_join",pjno);	
	}
	printf("retval=%d\n",(int)retval);
	pthread_exit(NULL);
}

