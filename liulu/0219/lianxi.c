#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
int main(int argc, char* argv[])
{
    int fd[2];
    char buf[10];
    pipe(fd);
    pid_t pid = fork();
    if(pid < 0)
    {
	perror("fork error:");
	exit(1);
    }
    else if(pid == 0)
    {
	close(fd[0]);
	int i;
	for(i = 0; i < 10; i++)
	{
	    dprintf(fd[1], "----%d----\n", i);
	    sleep(5);
	}
	close(fd[1]);
    }
    else
    {
	close(fd[1]);
	int epfd = epoll_create(10);
	struct epoll_event event, events[10];

	fcntl(fd[0], F_SETFL, O_NONBLOCK);//she zhi fei zu se

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = fd[0];
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd[0], &event);
	while(1)
	{
	    int ret = epoll_wait(epfd, events, 10, -1);
	    if(ret < 0)
	    {
		perror("epoll_wait error:");
		exit(1);
	    }
	    while(1)
	    {
		int rr = read(events[0].data.fd, buf, 5);
		if(rr < 0)
		{
		    if(errno == EAGAIN)
		    {
			break;
		    }
		    perror("read error:");
		    exit(1);
		}
		else if(rr == 0)
		{
		    printf("dui duan guan bi\n");
		    exit(1);
		}
		else
		{
		    write(STDOUT_FILENO, buf, rr);
		}
	    }
	}
    }

    return 0;
}

