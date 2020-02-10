#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
void sys_err(char* str)
{
	perror(str);
	exit(1);
}

//void *mmap(void *addr, size_t length, int prot, int flags,
//                  int fd, off_t offset);
int main(int argc, char* argv[])
{
	char* ptr = NULL;
	ptr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(ptr == MAP_FAILED)
	{
		sys_err("mmap error");
	}

	pid_t pid = fork();
	if(pid < 0)
	{
		sys_err("fork error");
	}
	else if(pid == 0)
	{
		strcpy(ptr, "*****-------");
		printf("child ptr = %s\n", ptr);
	}
	else
	{
		usleep(10);
		printf("parent ptr = %s\n", ptr);	
		int wr = wait(NULL);
		printf("wait pid = %d\n", wr);
	}

	int mur = munmap(ptr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
	}
	return 0;
}
