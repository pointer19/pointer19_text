/**********************************************************************/
/*******************************welcome********************************/
/********************************Mr.M**********************************/
/**********************************************************************/
//守护进程创建
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<sys/stat.h>
#include<fcntl.h>
void sys_err(const char* str)
{
	perror(str);
	exit(1);	
}


int main(int argc, char* argv[])
{

	int ret;
	pid_t pid=fork();
	if(pid<0)
	{
		sys_err("fork error");
	}
	else if(pid ==0)
	{
		//更改会话id
		pid=setsid();
		if(pid<0)
		{
			sys_err("setsid error");
		}
		//更改工作目录
		ret=chdir("/home/wfc/pointer19_text");
		if(ret<0)
		{
			sys_err("chdir error");
		}
		//更改umask掩码
		umask(022);

		close(STDIN_FILENO);
	
		int fd = open("/dev/null",O_RDWR);
		if(fd<0)
		{
			sys_err("open error");
		}
		dup2(STDOUT_FILENO,fd);
		dup2(STDERR_FILENO,fd);
		while(1);
	}
	else
	{
		//父进程退出
		exit(1);
	}

	return 0;
}

/***********************************************************************/
/*********************************end***********************************/
/***********************************************************************/

