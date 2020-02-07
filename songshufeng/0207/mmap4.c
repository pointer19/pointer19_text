/**********************************************************************/
/*******************************pointer********************************/
/********************************Mr.W**********************************/
/**********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/mman.h>

void sys_err(char* str)
{
	perror(str);
	exit(1);
}

//void *mmap(void *addr, size_t length, int prot, int flags,
//                  int fd, off_t offset);
int main(int argc, char* argv[])
{
	char* ptr = NULL;
	int fd = open("./mmap.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	//ftruncate(fd, 200);
	lseek(fd, 19999, SEEK_END);
	write(fd, "\0", 1);
	ptr = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
	if(ptr == MAP_FAILED)
	{
		sys_err("mmap error");
	}
	
	strcpy(ptr, "123456789");
	printf("ptr = %s\n", ptr);
	int mur = munmap(ptr, 100);
	if(mur < 0)
	{
		sys_err("munmap error");
	}
	close(fd);
	return 0;
}

/***********************************************************************/
/*********************************end***********************************/
/***********************************************************************/

