#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
#define STR "123456\n"
int main(int argc,char * argv[])
{
char *ptr =NULL;
ptr=(char *)mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
if(ptr==MAP_FAILED)
{
perror("mmap error");
exit(1);
}
int pid =fork();
if(pid<0)
{
perror("fork error");
exit(1);
}
else if(pid ==0)
{
printf("child str =%s\n",ptr);
}
else
{
usleep(10);
printf("parent str =%s\n",ptr);
int wr =wait(NULL);
printf("wait pid =%d\n",wr);
}
int mur =munmap(ptr ,100);
if(mur <0)
{
perror("munmap error");

}

return 0;

}
