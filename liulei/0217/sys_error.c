
#include "sys_error.h"
void sys_error(char *strerror)
{
	perror(strerror);
	exit(1);
}




