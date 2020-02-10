#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
void sys_err(char *str)
{
    perror(str);
    exit(1);
}
void catch_sig(int signum)
{
    pid_t wpid;
 //   wpid = wait(NULL);
//    if(wpid < 0)
 //   {
//	sys_err("wait error");
 //   }
//    while((wpid = wait(NULL)) > 0)
    while((wpid = waitpid(-1, NULL, WNOHANG)) > 0)
    {
	printf("catch child pid = %d\n", wpid);
    }
}
int main(int argc, char* argv[])
{
    pid_t pid;
    int i;
    sigset_t myset;
    sigemptyset(&myset);
    sigaddset(&myset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &myset, NULL);
    for(i = 0; i < 10; i++)
    {
	pid = fork();
	if(pid == 0)
	{
	    break;
	}
	else if(pid < 0)
	{
	    sys_err("fork error");
	}
    }
    if(i == 10)
    {
	struct sigaction act;
	act.sa_handler = catch_sig;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
	sigprocmask(SIG_UNBLOCK, &myset, NULL);
	printf("this is parent pid = %d\n",getpid());
	while(1);
    }
    else
    {
	printf("this is child pid = %d\n",getpid());
    }
    return 0;
}

