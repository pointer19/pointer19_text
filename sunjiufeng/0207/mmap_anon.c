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
	mr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if(mr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	pid_t pid = fork();
	int n = 0;
	if(pid < 0)
	{
		sys_err("fork error");
	}
	else if(pid == 0)
	{
		while(1)
		{
			sprintf(mr, "----%d----", n++);
			printf("child mr = %s\n", mr);
			sleep(1);
		}
	}
	else
	{
		while(1)
		{
			printf("parent mr = %s\n", mr);
			sleep(1);
		}
	}
	int mur = munmap(mr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
		exit(1);
	}
	return 0;
}

