#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#define STR "123456\n"
void sys_err(char *str,int ernu)
{
fprintf(stderr,"%s:%s",str,strerror(ernu));
exit(1);

}
void *  fun1()
{
return NULL;
}
void * pthread_fun(void *arg)
{
printf("pid =%d,tid=%lu\n",getpid(),pthread_self());
fun1();
printf("++++++++\n");
return NULL;
}
int main(int argc,char * argv[])
{
pthread_t tid;
pthread_create(&tid,NULL,pthread_fun,NULL);
sleep(2);
printf("-------\n");
return 0;
}
