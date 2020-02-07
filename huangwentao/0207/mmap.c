#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc,char* argv[])
{
char* bf=NULL;
int fd=open("t.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
lseek(fd,200,SEEK_END);
write(fd,"1",1);
bf=mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
while(1)
{
sprintf(bf,"asdfghjkl");
printf("h=%s\n",bf);	
sleep(1);
}

return 0;
}

