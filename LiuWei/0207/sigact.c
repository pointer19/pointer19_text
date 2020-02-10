#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/mman.h>
void sys_err(char* str)
{
    perror(str);
    exit(1);
}
void catch_sig(int signum)//回调函数
{
    printf("回调函数\n");
    while(1)
    {   
        pid_t pid = waitpid(0,NULL,WNOHANG);
        if(pid==0)
        {
            break;
        }
        if(pid==-1)
            exit(1);
        printf("pid = %d\n",pid);
    }
}

int main(int argc, char* argv[])
{
    int i=0;
    for(;i<10;i++)
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            sys_err("fork error");
            exit(1);
        }
        else if(pid == 0)
        {
            break;    
        }
        printf("%d...",i);
    }
    if(i==10)
    {
        struct sigaction act, oldact;
        act.sa_handler = catch_sig;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);  
        sigaction(SIGCHLD, &act, &oldact);

    
        for(i=0;;i++)
        {
            printf("%d\n",i);
            sleep(1);
        }
    }
    else
        sleep(3);
    return 0;
}
