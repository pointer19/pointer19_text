#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>
void sys_err(char* str)
{
    perror(str);
    exit(1);
}
int main(int argc,char *argv[])
{
    char *ptr =NULL;
    int fd = open("./mmap.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    lseek(fd, 199, SEEK_END);
    write(fd, "\0", 1);
    ptr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
        strcpy(ptr, "*****-------");
        printf("child ptr = %s\n", ptr);
    }
    else
    {
        usleep(10);
        printf("parent ptr = %s\n", ptr);   
        int wr = wait(NULL);
        printf("wait pid = %d\n", wr);
    }
    int mur = munmap(ptr, 100);
    if(mur < 0)
    {
        sys_err("munmap error");
    }
    close(fd);
    return 0;
}
