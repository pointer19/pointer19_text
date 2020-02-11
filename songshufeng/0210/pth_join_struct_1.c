#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void sys_err(const char* str,int errno)
{
    fprintf(stderr, "%s: %s", str, strerror(errno));
    exit(1);
}
struct student
{
    int no;
    char name[10];
};
void* pthread_fun(void * arg)//线程主函数
{
    struct student* song = (struct student*)arg;
    song->no = 100;
    strcpy(song->name, "song");
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    return song;
}
int main(int argc, char* argv[])//主线程
{
    pthread_t tid1;
    struct student* xiaohua;
    struct student* retval;
    pthread_create(&tid1, NULL, pthread_fun, (void *)xiaohua);
    pthread_join(tid1,(void**)&retval);//int ** 转化为 void ** 
    printf("%d\n",retval-> no);
    pthread_exit(NULL); 
}

