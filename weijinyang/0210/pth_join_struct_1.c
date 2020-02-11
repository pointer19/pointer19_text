#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void sys_err(const char* str,int errno)
{
//    strerror(errno);//fan yi  cuo wu hao
    fprintf(stderr, "%s: %s", str, strerror(errno));
    exit(1);
}
struct student
{
    int no;
    char name[10];
};
void* pthread_fun(void * arg)//xian  cheng  zhu  han  shu  
{//zi  xian  cheng
    struct student* xiaoxiao = (struct student*)arg;
    xiaoxiao->no = 100;
    strcpy(xiaoxiao->name, "xiaoxiao");
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    return xiaoxiao;
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
    pthread_t tid1;
    struct student* xiaohua;
    struct student* retval;
    pthread_create(&tid1, NULL, pthread_fun, (void *)xiaohua);
     
    pthread_join(tid1,(void**)&retval);//int ** zhuan void ** 
    printf("%d\n",retval-> no);
    pthread_exit(NULL); 
}

