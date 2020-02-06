#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
	int fd[2];
	char buf[1024];
	int pr= pipe(fd);
	pid_t pid=fork();
	if(pid==0)
	{
		close(fd[0]);
//		execlp("ls","ls","-l",NULL);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		execlp("ls","ls","-l",NULL);
	}
	else if(pid>0)
	{
		close(fd[1]);
	//	execlp("wc","wc","-l",NULL);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);
		execlp("wc","wc","-l",NULL);
	}

}
