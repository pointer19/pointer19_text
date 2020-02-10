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
			int status;
      for(int i=0;i<10;i++)
			{
           int wr= wait(&status);
           if(wr<0)
					 {
							perror("wait error");
							exit(1);
					 }
					 printf("wait pid=%d\n",wr);
			}
}
int main(int agrc,char* agrv[])
{
    pid_t pid;
		int i;
		int status;
		int wr;
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
					 printf("wait pid =%d\n",wr);
		}
		else
		{
					sleep(i);
          printf("child pid=%d\n",getpid());

		}
		return 0;
}

