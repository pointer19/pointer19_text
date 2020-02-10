#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
int main(int argc, char* argv[])
{
	char buf[1024];
	int i = 0;
	int fd = open("myfifo", O_RDWR);
	while(1)
	{
		sprintf(buf, "hello %dth\n", i++);
		write(fd, buf, strlen(buf));
		sleep(1);
	}
	return 0;
}

