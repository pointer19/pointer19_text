#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
int main(int argc,char* argv[])
{
int fd=open("./m.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
ftruncate(fd,200);
char *pr=mmap(NULL,100, PROT_READ| PROT_WRITE,MAP_SHARED,fd,0);
int fk=fork();

if(fk<0)
{
perror("fork error");exit(1);	
}
else if(fk>0)
{
 strcpy(pr,"child");
 printf("fafa=%s\n",pr);
 wait(NULL);	
}
else
{
usleep(10);
 printf("%s=fd\n",pr);	
}
return 0;
}

