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
		int fd=open("./mmap.txt",O_RDWR | O_CREAT | O_TRUNC,0644);
		lseek(fd,199,SEEK_END);
	  write(fd,"\0",1);
		ptr=(char*)mmap(NULL,100,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
		if(ptr==MAP_FAILED)
		{
        perror("mmap error");
				exit(1);
		}
		int n=0;
		while(1)
		{
				sprintf(ptr,"******n=%d",n++);
				printf("write ptr=%s\n",ptr);
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

