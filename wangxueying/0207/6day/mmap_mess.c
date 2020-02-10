#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
void sys_err(char *p)
{
	perror(p);
	exit(1);
}
int main(int argc, char *argv[])
{
	char *ptr=NULL;
	int fd=open("text.txt",O_RDWR|O_TRUNC|O_CREAT,0644);
//	lseek(fd,100,SEEK_END);
//	write(fd,"1",1);
	ftruncate(fd,100);
	ptr=mmap(NULL,50,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)
	{
		sys_err("mmap error");
	}
	int pid=fork();
	if(pid<0)
	{
		sys_err("fork error");
	}
	else if(pid==0)
	{
		strcpy(ptr,"--------------");
		printf("child ptr=%s\n",ptr);
	}
	else
	{
		usleep(10);
		printf("parent ptr =%s\n",ptr);
		int wr=wait(NULL);
		printf("wait pid= %d\n",wr);
	}

	int unm=munmap(ptr,50);
	if(unm<0)
	{
		sys_err("munm error");
	}
	close(fd);
	return 0;
}

