#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#define STR "123456\n"
void sys_err(char *str)
{
perror(str);
exit(1);
}
void catch_child(int signum)
{

int wpid;
while((wpid=wait(NULL)) != -1)
{
printf("wait pid =%d\n",wpid);
}
}
int main(int argc,char * argv[])
{
int pid;
int i;
for(i=0;i<10;i++)
{
pid=fork();
if(pid<0)
	{
	sys_err("fork error");
	}
else if(pid ==0)
{
break;
}
}
if(i==10)
{
struct sigaction act;
act.sa_handler =catch_child;
sigemptyset(&act.sa_mask);
act.sa_flags =0;
sigaction(SIGCHLD,&act,NULL);

printf("i am parent ,pid =%d\n",getpid());

while(1);
}
else
{
sleep(1);
printf("child pid =%d,ppid=%d\n",getpid(),getppid());

}

return 0;

}
