#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

#define STR "123456\n"
int main(int argc,char * argv[])
{
int ar1,ar2;
ar1 = alarm(10);
sleep(2);
ar2 = alarm(5);
printf("ar1=%d,ar2=%d\n",ar1,ar2);
while(1);
return 0;

}