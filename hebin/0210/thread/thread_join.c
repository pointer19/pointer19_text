#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<pthread.h>
void sys_errr(char *str,int ernu)
{
	fprintf(stderr,"%s: %s",str,strerror(ernu));
	exit(1);
}

struct student
{
	int num;
	char name[256];
};

void* pthread_fun(void *arg)
{
	struct student *stu=(struct student*)arg;
	stu->num = 123;
	strcpy(stu->name,"xuesheng");
//	struct student *stu1; 
//	stu1 = (void *)malloc(sizeof(stu1));
//	stu1->num = 123;
//	strcpy(stu1->name,"tongxue");
	return (void*)stu;
}
int main(int argc, char* argv[])//主线程死的太快，所以tid无法输出 return 0结束了当前进程，内存空间被释放，线程共享的内存空间也被释放
{
	pthread_t tid;
	struct student *retval;
	struct student stu;
	
	pthread_create(&tid,NULL,pthread_fun,(void *)&stu);//值传递
	pthread_join(tid,(void**)&retval);
	printf("retval = %d retvalname=%s\n",retval->num,retval->name);

	pthread_exit(NULL);
}

