#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

void sys_err(char* str)
{
    perror(str);
    exit(1);
}

int main(int argc, char* argv[])
{
    int ret,fd;
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
	sys_err("fork error");
    }
    else if(pid > 0)
    {
	exit(1);
    }
    else
    {
	pid = setsid();
	if(pid < 0)
	{
	    sys_err("setsid error");
	}
	ret = chdir("/home/wjy/pointer_19");
	if(ret < 0)
	{
	    sys_err("chdir error");
	}
	umask(010);
	close(STDIN_FILENO);

	fd = open("/dev/null", O_RDWR);
	if(fd < 0)
	{
	    sys_err("open error00");
	}
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	while(1);
    }
    return 0;
}

