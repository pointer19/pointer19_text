#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
void sys_err(char *p)
{
	perror(p);
	exit(1);
}
int main(int argc, char *argv[])
{
	char *ptr=NULL;
	int fd=open("text.txt",O_RDONLY);
//	lseek(fd,100,SEEK_END);
//	write(fd,"1",1);
	ftruncate(fd,100);
	ptr=mmap(NULL,50,PROT_READ,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)
	{
		sys_err("mmap error");
	}
	int n=0;
	while(1)
	{

		printf("ptr = %s\n",ptr);
		sleep(1);
	}
	int unm=munmap(ptr,50);
	if(unm<0)
	{
		sys_err("munm error");
	}
	close(fd);
	return 0;
}

