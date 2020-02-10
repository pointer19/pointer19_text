#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
	int a2;
	a2=alarm(1);
	int i=1;
	while(1)
	{
		printf("%d\n",i++);
	}
	return 0;
}

