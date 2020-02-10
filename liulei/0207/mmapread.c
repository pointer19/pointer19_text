#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/mman.h>



int main(int argc, char *argv[])
{
	int fd = open("./mmap.txt", O_RDONLY);
	char *m=(char *)mmap(NULL, 100, PROT_READ, MAP_SHARED, fd, 0);
	if(m == MAP_FAILED)
	{
		perror("mmap error:");
		exit(1);
	}
	while(1)
	{
		printf("str=%s", m);
		sleep(1);
	}
	int mun = munmap(m, 100);
	if(mun < 0)
	{
		perror("munmap error:");
		exit(1);
	}
	close(fd);
	return 0;
}

