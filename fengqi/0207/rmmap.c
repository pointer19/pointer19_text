#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>


int main(int agrc,char* agrv[])
{
    char* ptr =NULL;
		int fd=open("./mmap.txt",O_RDONLY);
		ptr=(char*)mmap(NULL, 100, PROT_READ, MAP_SHARED, fd, 0);
		if(ptr==MAP_FAILED)
		{
        perror("mmap error");
				exit(1);
	  }
		while(1)
		{
				printf("read ptr= %s\n",ptr);
        sleep(1);
	  }
		int mur=munmap(ptr,100);
		if(mur<0)
		{
        perror("unmmap error");
				exit(1);
		}
		close(fd);
		return 0;
}

