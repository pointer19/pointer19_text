#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    char buf[1024];
    int i=0;
    int fd = open("myfifo", O_RDWR);
    while(1)
    {
	sprintf(buf, "this is no %d\n", i++);
	write(fd, buf, strlen(buf));
	sleep(1);
    }
    return 0;
}

