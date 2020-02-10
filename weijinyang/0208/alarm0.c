#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    int ar1;
    ar1 = alarm(1);
    int i = 0;
    while(1)
    {
	printf("i = %d\n", i++);
    }
    return 0;
}

