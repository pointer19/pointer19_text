#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char* argv[])
{
 int fd;
 fd=open("./lswc.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
 if(fd<0)
 {
	 perror("file error:");
	 exit(1);
 }
 int fr=fork();
 if(fr<0)
 {
	 perror("fork error:");
	 exit(1);
 }
 else if(fr>0)
 {
 	dup2(fd,STDOUT_FILENO);
	execlp("ls","ls","-l",NULL);	 
 }
 else
 {
	sleep(1);
 	//dup2(fd,STDIN_FILENO);
	execlp("wc","wc","-l","lswc.txt",NULL);	 
 }


return 0;
}

