#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void sys_err(char* str,int erno)
{
		printf("%s:%s",str,strerror(erno));
}
struct student
{
	 int num;
	 char name[200];
};
void* pthread_fun(void *arg)
{
     struct student* fengqi=(struct student*)arg;
		 fengqi->num=666;
		 strcpy(fengqi->name,"fengqi");
		 return (void*)fengqi;
}
int main(int agrc,char* agrv[])
{
		struct student * retval;
		struct student fengqi1;
    pthread_t tid1;
		pthread_create(&tid1,NULL,pthread_fun,(void*)&fengqi1);
		pthread_join(tid1,(void**)&retval);
		printf("num=%d,name=%s\n",retval->num,retval->name);//zusehuishou
		pthread_exit(NULL);
}

