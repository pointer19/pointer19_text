#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>

void print_sig(sigset_t *myset)
{
	for(int i = 1; i < 32; i++)
	{
		if(sigismember(myset, i) == 1)
		{
			printf("1");
		}
		else if(sigismember(myset, i) == 0)
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
	
	sigprocmask(SIG_BLOCK, &myset, &oldset);

	while(1)
	{
		sigpending(&myset);
		print_sig(&myset);
		sleep(1);
	}

	return 0;
}

