#include "Signal.h"
#include "sys_error.h"
void c_signal(int a)
{
	pid_t pid;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("wait : %d\n", pid);
	}
}
void Signal() 
{
	struct sigaction act;
	act.sa_handler = c_signal;
	sigemptyset(&act.sa_mask);
	act.sa_flags=SA_RESTART;
	sigaction(SIGCHLD, &act, NULL);
}


