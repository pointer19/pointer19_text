#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
#include<sys/wait.h>
int main(int argc,char* argv[])
{//写端
	int fd;
	char *ptr=NULL;
	fd=open("./aa.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
	lseek(fd,200,SEEK_END);  //open之后为0,进行扩充
	write(fd,"\0",1);//lseek伴随IO操作
	ptr=(char*)mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(ptr==MAP_FAILED)//必须进行返回值检查
	{
		perror("map error");	
		exit(1);
	}
	int n=0;
	while(1)
	{
	sprintf(ptr,"------------%d---",n++);
	printf("I am  write=%s\n",ptr);
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

