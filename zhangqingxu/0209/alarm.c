#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
	int a1,a2;
	a1=alarm(5);
	sleep(3);
	a2=alarm(1);
	printf("a1=%d a2=%d\n",a1,a2);	
	while(1);
	return 0;
}

