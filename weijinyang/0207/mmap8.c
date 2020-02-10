#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char* argv[])
{
    char *ptr = NULL;
    int fd = open("./mmap.txt", O_RDONLY, 0644);
    ptr = (char *)mmap(NULL, 100, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(ptr == MAP_FAILED)
    {
	sys_err("mmap error");
    }
    strcpy(ptr, "_________________");
    printf("ptr = %s\n", ptr);
    int munm = munmap(ptr, 100);
    if(munm < 0)
    {
	sys_err("munmap error");
    }
    close(fd);
    return 0;
}

