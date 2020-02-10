#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	char buf[1024];
	int rr;
	int fd = open("myfifo", O_RDONLY);
	while(1)
	{
		rr = read(fd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, rr);
		sleep(1);
	}
	return 0;
}

