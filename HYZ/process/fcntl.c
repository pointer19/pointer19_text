#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc,char* argv[])
{
	int fr;
	int rr;
	char buf[1024];
	fr=fcntl(STDIN_FILENO,F_GETFL);
	fr|=O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,fr);
	rr=read(STDIN_FILENO,buf,10);
	if(rr<0)
	{
		if(errno!=EAGAIN)
		{
			printf("error");
			exit(1);		
		}
		else
		{
			perror("wenjianweifeizuse,wushuju");
			exit(1);	
		}
	}
	write(STDOUT_FILENO,buf,rr);

	return 0;	
}

