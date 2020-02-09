#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    int ar1,ar2;
    ar1 = alarm(5);//the first return value is 0;
    sleep(2);//sleep 2s , alarm(5) have 3s
    ar2 = alarm(10);//ar2 = 3, wait 10s then send alarm signal
    printf("ar1 = %d, ar2 = %d\n",ar1, ar2);
    while(1);
    return 0;
}

