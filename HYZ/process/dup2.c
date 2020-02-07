#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#define s1 "123456"
#define s2 "999"

int main(int argc,char* argv[])
{
	int fd1=open("./1.txt",O_RDWR|O_CREAT,0644);
	dup2(fd1,STDOUT_FILENO);
	printf("999");
	close(fd1);
	
	return 0;	
}

