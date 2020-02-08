#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	char buf[1024];
	int i=0;
	int fd = open("myfifo", O_RDWR);
	while(1)
	{
		sprintf(buf, "hello %dth\n", i++);
		write(fd, buf, strlen(buf));
		sleep(1);
	}
	return 0;
}

