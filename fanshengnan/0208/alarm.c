#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
int main()
{
	int ar1,ar2;
	ar1=alarm(5);
	sleep(2);
	ar2=alarm(10);
	printf("ar1=%d,ar2=%d\n",ar1,ar2);
	while(1);
	return 0;
}
