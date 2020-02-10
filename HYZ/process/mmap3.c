#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
#include<sys/wait.h>
int main(int argc,char* argv[])
{//读端
	int fd;
	char *ptr=NULL;
	fd=open("./aa.txt",O_RDONLY);
	ptr=(char*)mmap(NULL,100,PROT_READ,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)//必须进行返回值检查
	{
		perror("map error");	
		exit(1);
	}
	pid_t pid=fork();//在建立映射区之后fork
	if(pid<0)
	{
		perror("fork error");
		exit(1);	
	}
	while(1)
	{
	printf("I am read=%s\n",ptr);
	sleep(1);
	}
	int mur=munmap(ptr,100);//对映射区进行回收
	if(mur<0)
	{
		perror("mumap error");
		exit(1);
	}
	close(fd);
	return 0;	
}

