#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#define STR "123456\n"

void print(sigset_t *myset)
{
int i;
for(i=1;i<32;i++)
{
if(sigismember(myset,i)==1)
{
printf("1");
}
else{
printf("0");
}

}

printf("\n");
}
int main(int argc,char * argv[])
{
sigset_t myset,old_set;
sigemptyset(&myset);
sigaddset(&myset,SIGINT);
sigprocmask(SIG_BLOCK,&myset,&old_set);
while(1)
{
sigpending(&myset);
print(&myset);
sleep(1);
}
return 0;

}
