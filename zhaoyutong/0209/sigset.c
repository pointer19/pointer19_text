#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
void print_pen(sigset_t* myset)
{
     int i;
     for(i = 1; i < 32; i++)
     {
      if(sigismember(myset, i)==1)
      {
	 printf("1");
      }
      else
	{
	printf("0");
	}
    }
	printf("\n");
}
int main(int argc, char* argv[])
{
	sigset_t myset, oldset;
	sigemptyset(&myset);
	sigaddset(&myset, SIGINT);
	sigaddset(&myset, SIGBUS);
	sigaddset(&myset, SIGSEGV);
	sigaddset(&myset, SIGALRM);
	sigaddset(&myset, SIGKILL);
	sigprocmask(SIG_BLOCK, &myset, &oldset);
	while(1)
	{
		sigpending(&myset);
		print_pen(&myset);
		sleep(1);
	}
	return 0;
}
