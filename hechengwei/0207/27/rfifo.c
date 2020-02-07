#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

#define STR "123456\n"
int main(int argc,char * argv[])
{
char buf[1024];
int i=0;
int  fd =open("myfifo",O_RDWR);
while(1)
{
rr=read(fd ,buf ,sizeof(buf));
write(STDOUT_FILENO,buf,rr);
sleep(1);
}
return 0;

}
