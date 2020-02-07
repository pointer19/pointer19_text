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
int fd=open("t.txt",O_RDWR);
bf=mmap(NULL,100,PROT_READ,MAP_SHARED,fd,0);
while(1)
{
	printf("h=%s\n",bf);
	sleep(1); 
}


return 0;
}

