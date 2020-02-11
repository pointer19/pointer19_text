#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>

void catch_wait(int signo)
{
			pid_t wpid;
			while(wpid=waitpid(-1,NULL,WNOHANG)>0)
			{
							printf("wait child pid=%d\n",wpid);
			}
}
int main(int agrc,char* agrv[])
{
		pid_t pid;
		int i;
		sigset_t myset;
		sigemptyset(&myset);
		sigaddset(&myset,SIGCHLD);
		sigprocmask(SIG_BLOCK,&myset,NULL);
		for(i=0;i<10;i++)
		{
        pid=fork();
				if(pid<0)
				{
             perror("fork error");
						 exit(1);
				}
				else if(pid==0)
				{
            break;
				}
		}
		if(i==10)
		{
           struct sigaction act, oldact;
					 act.sa_handler=catch_wait;
					 act.sa_flags=0;
           sigemptyset(&act.sa_mask);
					 sigaction(SIGCHLD,&act,&oldact);
           sigprocmask(SIG_UNBLOCK,&myset,NULL);
					 printf("I'M parent  pid =%d\n",getpid());
					 while(1)
					 {
							printf("********\n");
							sleep(1);
					 }
		}
		else
		{
					sleep(i);
          printf("child pid=%d\n",getpid());

		}
		return 0;
}

