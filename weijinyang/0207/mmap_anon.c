#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
void sys_err(char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char* argv[])
{
    char *ptr = NULL;
    ptr = (char *)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(ptr == MAP_FAILED)
    {
	sys_err("mmap error");
    }
    pid_t pid = fork();
    if(pid < 0)
    {
	sys_err("fork error");
    }
    else if(pid == 0)
    {
	strcpy(ptr, "______________");
	printf("child ptr = %s\n", ptr);
    }
    else
    {
	usleep(10);
	printf("parent ptr = %s\n", ptr);
	wait(NULL);
    }
    int munm = munmap(ptr, 100);
    if(munm < 0)
    {
	sys_err("munmap error");
    }
    return 0;
}
