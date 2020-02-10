#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}
int main()
{
	char* ptr = NULL;
	int fd = open("./mmap.txt", O_RDONLY | O_CREAT | O_TRUNC, 0644);
	int len = lseek(fd, 199, SEEK_END);
	if(len<0)
	{
		sys_err("lseek error");
	}
	write(fd, "\0", 1);
	ptr = (char*)mmap(NULL, 100, PROT_READ ,MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	
	strcpy(ptr, "123456789");
	printf("ptr = %s\n", ptr);
	int mur = munmap(ptr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
	}
	close(fd);
	return 0;
}
