#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

#define STR "123456\n"
int main(int argc,char * argv[])
{

int pid;
pid =fork();
if(pid<0)
{
perror("fork error");
exit(1);
}
else if(pid >0)
{
exit(1);

}
else
{
pid =setsid();
if(pid<0)
{
perror("setseid error");
exit(1);
}
ret =chdir("/home/yinlan/27");
if(ret<0)
{
perror("chdir error");
exit(1);
}
umask(022);
close(STDIN_FILENO);
int fd =open("/dev/null",O_RDWR);
if(fd<0)
{

perror("open error");
exit(1);
}
dup2(fd,STDOUT_FILENO);
dup2(fd,STDERR_FILENO);
while(1);
}
return 0;

}
