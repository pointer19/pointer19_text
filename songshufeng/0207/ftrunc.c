#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main()
{
	int fd = open("./tesx.c", O_RDWR | O_CREAT | O_TRUNC, 0644);
	ftruncate(fd, 200);
	return 0;
}

