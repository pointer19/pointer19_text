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
//	struct student* xiaoming=(struct student*) arg;
	struct student *qinyuan;
	qinyuan=malloc(sizeof(qinyuan));
	qinyuan->num=123;
	strcpy(qinyuan->name,"qinyuan");
	return qinyuan;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	struct student *xiaoming;
	struct student *retval;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_join(tid,(void **)&retval);
	printf("retval num=%d,name=%s\n",retval->num,retval->name);
	//return 0;
	pthread_exit(NULL);
}

