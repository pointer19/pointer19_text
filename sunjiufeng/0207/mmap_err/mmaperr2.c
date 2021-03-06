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
	int fd = open("./mmap.txt",O_RDONLY);
	mr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(mr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	strcpy(mr, "123456789");
	printf("mr = %s\n", mr);
	int mur = munmap(mr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
		exit(1);
	}
	close(fd);
	return 0;
}

