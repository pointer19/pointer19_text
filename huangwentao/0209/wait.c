#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
void sig_catch(int num)
{
	while(waitpid(0,NULL,WNOHANG)>0)
	{
		printf("pid =%d\n",num);	
	}
}
int main(int argc,char* argv[])
{

pid_t pid;
sigset_t mymask;
sigemptyset(&mymask);
sigaddset(&mymask,SIGCHLD);
sigprocmask(SIG_BLOCK,&mymask,NULL);
int i;
for(i=0;i<10;i++)
{
	
	pid=fork();	
	if(pid==0) break;
}
if(i==10)
{
struct sigaction new;
new.sa_flags=0;
new.sa_handler=sig_catch;
sigemptyset(&new.sa_mask);
sigaction(SIGCHLD,&new,NULL);
sigprocmask(SIG_UNBLOCK,&mymask,NULL);

while(1);
}
else
{
	printf("child =%d\n",getpid());	
}
return 0;
}

