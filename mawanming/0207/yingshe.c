#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	char* ptr=NULL;
	int fd=open("/dev/zero",O_RDWR);
	ptr=(char*)mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	pid_t pid=fork();
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	if(pid==0)
	{
		strcpy(ptr,"*****\0");
		printf("child ptr=%s\n",ptr);
	}
	if(pid>0)
	{
		wait(NULL);
		printf("parent ptr=%s\n",ptr);
	}
	return 0;
}
