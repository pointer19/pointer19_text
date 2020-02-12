#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
int a=0;
void* add(void* argc)
{
 for(int i=0;i<10000;i++)
	{
		a++;
		printf("---%d----\n",a);
	}
	
}
int main(int argc,char* argv[])
{
pthread_t tid;
pthread_create(&tid,NULL,add,NULL);
pthread_detach(tid);


 for(int i=0;i<10000;i++)
	{
		a++;
		printf("---%d----\n",a);
	}

return 0;
}

