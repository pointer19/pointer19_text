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
}

int main(int argc,char * argv[])
{
signal(SIGINT,catch_sig);
while(1);

return 0;

}
