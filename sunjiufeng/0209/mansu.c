#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

void catch_sig(int signum)
{
	printf("-----%d------\n", signum);
}

int main(int argc, char* argv[])
{
	int fd[2];
	char buf[1024];
	int pr = pipe(fd);
	if(pr < 0)
	{
		sys_err("pipe error");
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		sys_err("fork error");
	}
	else if(pid == 0)
	{
		sleep(2);
		while(1)
		{
			printf("++++++++\n");
			sleep(1);
		}
	}
	else 
	{
		struct sigaction act;
		act.sa_handler = catch_sig;
		act.sa_flags = 0;
		//act.sa_flags = SA_RESTART;
		sigemptyset(&act.sa_mask);

		sigaction(SIGINT, &act, NULL);

		printf("parent \n");
		read(fd[0], buf, 1024);
		while(1)
		{
			printf("=========\n");
			sleep(1);
		}
	}

	return 0;
}

