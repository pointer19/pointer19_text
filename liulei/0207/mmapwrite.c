#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/mman.h>



int main(int argc, char *argv[])
{
	int i=0;
	int fd = open("./mmap.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	lseek(fd, 199, SEEK_END);
	write(fd, "\0", 1);
	char *m=(char *)mmap(NULL, 100, PROT_WRITE, MAP_SHARED, fd, 0);
	if(m == MAP_FAILED)
	{
		perror("mmap error:");
		exit(1);
	}
	while(1)
	{
		sprintf(m, "---%d---\n", i++);
		printf("str=%s", m);
		sleep(1);
	}
	int mun=munmap(m, 100);
	if(mun < 0)
	{
		perror("mun error:");
		exit(1);
	}
	close(fd);
	return 0;
}

