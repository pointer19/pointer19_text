#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
struct student
{
	int num;
	char name[256];
};
void sys_error(char *str,int ernu)
{
	fprintf(stderr,"%s:,%s:",str,strerror(ernu));
	exit(1);
}
void* pthread_fun(void *arg)
{
	struct student* xiaoming=(struct student*) arg;
	xiaoming->num=123;
	strcpy(xiaoming->name,"xiaoming");
	return (void *)xiaoming;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	struct student xiaoming;
	struct student *retval;
	pthread_create(&tid,NULL,pthread_fun,(void *)&xiaoming);
	pthread_detach(tid);
	int jor=pthread_join(tid,(void **)&retval);
	if(jor!=0)
	{
		sys_error("pthread_join error",jor);
	}
	printf("retval num=%d,name=%s\n",retval->num,retval->name);
	pthread_exit(NULL);
}

