#include "socket.h"
#include "communication.h"
#include "Signal.h"
#define INET_ADDRLEN 20
int main(int argc, char *argv[])
{
	char str[INET_ADDRLEN];
	struct sockaddr_in ser_addr,cli_addr;
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(Port);
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int scfd = Socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	
	Signal();

	setsockopt(scfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	socklen_t ser_addrlen = sizeof(ser_addr);
	Bind(scfd, &ser_addr, ser_addrlen);
	Listen(scfd, 128);


	while(1)
	{
		printf("..continue...\n");
	    socklen_t cliaddr_len = sizeof(cli_addr);
		int acfd = Accept(scfd, &cli_addr, &cliaddr_len);
		printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr, str, sizeof(str)),ntohs(cli_addr.sin_port));
		pid_t pid = fork();
		if(pid == 0)
		{
			close(scfd);
			communication(acfd);
			close(acfd);
		}
		else if(pid >0)
		{
			close(acfd);
		}
	}



	return 0;
}

