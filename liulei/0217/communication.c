#include "communication.h"
#include "sys_error.h"
void communication(int socketfd)
{
	char buf[100];
	while(1)
	{

		int rr = read(socketfd, buf, 100);
		if(rr == -1)
		{
			sys_error("read error:");
		}
		else if(rr == 0)
		{
			exit(1);
		}
		else
		{
			int re = write(STDOUT_FILENO, buf, rr);
			if(re == -1)
			{
				sys_error("write error:");
			}
		}
	}
}





