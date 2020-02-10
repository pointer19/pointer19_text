#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
int main()
{
	int ar1,i;
	ar1=alarm(1);
	i=0;
	while(1)
	{
		printf("i=%d\n",i++);
	}
	return 0;
}
