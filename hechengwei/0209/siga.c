#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#define STR "123456\n"
void catch_sig(int sig)
{
printf("catch sig %d\n",sig);
sleep(4);
printf("sleep 3\n");
}

int main(int argc,char * argv[])
{
struct sigaction act, oldset;

act.sa_handler =catch_sig;
act.sa_flags =0;
sigemptyset(&act.sa_mask);

sigaction(SIGINT,&act,&oldset);
while(1);

return 0;

}
