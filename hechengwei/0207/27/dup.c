#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

#define STR "123456\n"
int main(int argc,char * argv[])
{

int  fd2 =open("./3.txt",O_RDWR|O_CREAT,0644);
dup2(fd2,STDOUT_FILENO);
//write(STDOUT_FILENO,STR,7);
printf("---");
close(fd2);
return 0;

}
