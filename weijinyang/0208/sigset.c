#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
void print_pen(sigset_t* myset)
{
    int i = 1;
    while(i < 32)
    {
	if(sigismember(myset,i) == 1)
	{
	    printf("1");
	}
	else
	{
	    printf("0");
	}
	i++;
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
    }
    return 0;
}

