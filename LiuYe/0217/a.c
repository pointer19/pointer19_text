#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<fcntl.h>

#define BUF_SIZE 4096
#define SER_PORT 8000
void* tfun(void*arg)
{
	int rr;//read fanhuizhi
  int cfd=(int)arg;
	char buf[BUF_SIZE];
	while(1)
	{
		read(cfd,buf,BUF_SIZE);
	  if(rr<0)
		{
			perror("read error ");
			exit(1);
		
		}

		else if(rr==0)
		{
			printf(" client not connect\n");
			break;
		
		}
	//	write(STDOUT)
	}
	pthread_exit(NULL);

}

int main()
{
	int lfd,cfd;
	struct sockaddr_in ser_addr,cli_addr;
  socklen_t cli_len=sizeof(cli_addr);
  
  pthread_t tid;

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  ser_addr.sin_port=htons(SER_PORT);

	lfd=socket(AF_INET,SOCK_STREAM,0);

	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

	listen(lfd,128);

	while(1)
	{
		cfd=accept(lfd,(struct sockaddr*)&cli_addr,&cli_len);

		printf("client IP: %s, port:%d\n",inet_ntop(AF_INET,&cli_addr.sun_addr.s_addr,dst,64),ntohs(cli_addr.sin_port));//
		pthread_create(&tid,NULL,tfun,(viod*)cfd);
		pthread_detach(tid);//
		
	
	}
	return 0;
}
