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
	mr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(mr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		sys_err("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		strcpy(mr, "123456789");
		printf("child mr = %s\n", mr);
	}
	else
	{
		usleep(10);
		printf("parent mr = %s\n", mr);
		int wr = wait(NULL);
		if(wr < 0)
		{
			sys_err("wait error");
		}
		printf("wr = %d\n", wr);
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

