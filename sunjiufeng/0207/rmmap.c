#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/mman.h>

void sys_err(char c[])
{
	perror(c);
	exit(1);
}

int main(int argc, char* argv[])
{
	char *mr = NULL;
	int fd = open("./mmap.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	lseek(fd, 199, SEEK_END);
	write(fd, "\0", 1);
	mr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(mr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	while(1)
	{
		printf("read mr = %s\n", mr);
		sleep(1);
	}
	int mur = munmap(mr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
		exit(1);
	}
	close(fd);
	return 0;
}

